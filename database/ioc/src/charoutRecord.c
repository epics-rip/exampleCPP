/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/*
 * Author: 	Marty Kraimer
 * Date:	2018.04
 */ 
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "epicsPrint.h"
#include "alarm.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "devSup.h"
#include "errMdef.h"
#include "recSup.h"
#include "recGbl.h"
#include "menuYesNo.h"
#include "menuIvoa.h"
#include "menuOmsl.h"

#define GEN_SIZE_OFFSET
#include "charoutRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record(struct dbCommon *, int);
static long process(struct dbCommon *);
#define special NULL
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long get_units(DBADDR *, char *);
#define get_precision NULL
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double(DBADDR *, struct dbr_grDouble *);
static long get_control_double(DBADDR *, struct dbr_ctrlDouble *);
static long get_alarm_double(DBADDR *, struct dbr_alDouble *);

rset charoutRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};
epicsExportAddress(rset,charoutRSET);


static void checkAlarms(charoutRecord *prec);

static long init_record(struct dbCommon *pcommon, int pass)
{
    struct charoutRecord *prec = (struct charoutRecord *)pcommon;

    if (pass==0) return 0;
    recGblResetAlarms(prec);

    return 0;
}

static long process(struct dbCommon *pcommon)
{
    struct charoutRecord *prec = (struct charoutRecord *)pcommon;
    long status = 0;
    if(prec->drvh!=prec->drvl) {
        if(prec->val>prec->drvh) prec->val = prec->drvh;
        if(prec->val<prec->drvl) prec->val = prec->drvl;
    }
    checkAlarms(prec);
    recGblGetTimeStamp(prec);
    unsigned short monitor_mask = recGblResetAlarms(prec);
    monitor_mask |= DBE_VALUE;
    db_post_events(prec, &prec->val, monitor_mask);
    prec->pact=FALSE;
    return(status);
}

#define indexof(field) charoutRecord##field

static long get_units(DBADDR *paddr,char *units)
{
    charoutRecord *prec=(charoutRecord *)paddr->precord;

    if(paddr->pfldDes->field_type == DBF_LONG) {
        strncpy(units,prec->egu,DB_UNITS_SIZE);
    }
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    charoutRecord *prec=(charoutRecord *)paddr->precord;
    
    switch (dbGetFieldIndex(paddr)) {
        case indexof(VAL):
        case indexof(HIHI):
        case indexof(HIGH):
        case indexof(LOW):
        case indexof(LOLO):
        case indexof(LALM):
        case indexof(ALST):
        case indexof(MLST):
            pgd->upper_disp_limit = prec->hopr;
            pgd->lower_disp_limit = prec->lopr;
            break;
        default:
            recGblGetGraphicDouble(paddr,pgd);
    }
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    charoutRecord *prec=(charoutRecord *)paddr->precord;

    switch (dbGetFieldIndex(paddr)) {
        case indexof(VAL):
        case indexof(HIHI):
        case indexof(HIGH):
        case indexof(LOW):
        case indexof(LOLO):
        case indexof(LALM):
        case indexof(ALST):
        case indexof(MLST):
            /* do not change pre drvh/drvl behavior */
            if(prec->drvh > prec->drvl) {
                pcd->upper_ctrl_limit = prec->drvh;
                pcd->lower_ctrl_limit = prec->drvl;
            } else {
                pcd->upper_ctrl_limit = prec->hopr;
                pcd->lower_ctrl_limit = prec->lopr;
            }
            break;
        default:
            recGblGetControlDouble(paddr,pcd);
    }
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    charoutRecord    *prec=(charoutRecord *)paddr->precord;

    if(dbGetFieldIndex(paddr) == indexof(VAL)) {
         pad->upper_alarm_limit = prec->hihi;
         pad->upper_warning_limit = prec->high;
         pad->lower_warning_limit = prec->low;
         pad->lower_alarm_limit = prec->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(charoutRecord *prec)
{
    epicsInt32 val, hyst, lalm;
    epicsInt32 alev;
    epicsEnum16 asev;

    if (prec->udf) {
        recGblSetSevr(prec, UDF_ALARM, prec->udfs);
        return;
    }

    val = prec->val;
    hyst = prec->hyst;
    lalm = prec->lalm;

    /* alarm condition hihi */
    asev = prec->hhsv;
    alev = prec->hihi;
    if (asev && (val >= alev || ((lalm == alev) && (val >= alev - hyst)))) {
        if (recGblSetSevr(prec, HIHI_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* alarm condition lolo */
    asev = prec->llsv;
    alev = prec->lolo;
    if (asev && (val <= alev || ((lalm == alev) && (val <= alev + hyst)))) {
        if (recGblSetSevr(prec, LOLO_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* alarm condition high */
    asev = prec->hsv;
    alev = prec->high;
    if (asev && (val >= alev || ((lalm == alev) && (val >= alev - hyst)))) {
        if (recGblSetSevr(prec, HIGH_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* alarm condition low */
    asev = prec->lsv;
    alev = prec->low;
    if (asev && (val <= alev || ((lalm == alev) && (val <= alev + hyst)))) {
        if (recGblSetSevr(prec, LOW_ALARM, asev))
            prec->lalm = alev;
        return;
    }

    /* we get here only if val is out of alarm by at least hyst */
    prec->lalm = val;
    return;
}
