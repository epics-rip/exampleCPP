/*************************************************************************\
* Copyright (c) 1995-2010 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* This software is distributed subject to a Software License Agreement
* found in the file LICENSE that is included with this distribution.
\*************************************************************************/

/* runcontrolRecord.c - Record Support for the control record. */
/*
 *      Original Author: Claude Saunders
 *      Date:            10-18-95
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <alarm.h>
#include <callback.h>
#include <cantProceed.h>
#include <dbDefs.h>
#include <dbEvent.h>
#include <dbAccess.h>
#include <dbFldTypes.h>
#include <dbScan.h>
#include <devSup.h>
#include <epicsExport.h>
#include <errMdef.h>
#include <recSup.h>
#include <recGbl.h>
#include <epicsTimer.h>

#define GEN_SIZE_OFFSET
#include "runcontrolRecord.h"
#undef GEN_SIZE_OFFSET

struct rcPvt {
    CALLBACK callback;
    struct dbCommon *precord;
};

static void monitor(runcontrolRecord *);
static void reset(runcontrolRecord *);

/* Create RSET - Record Support Entry Table */
#define report NULL
#define initialize NULL
static long init_record();
static long process();
static long special();
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
#define get_units NULL
static long get_precision();
static long get_enum_str();
static long get_enum_strs();
static long put_enum_str();
#define get_graphic_double NULL
#define get_control_double NULL
#define get_alarm_double NULL

rset runcontrolRSET = {
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
epicsExportAddress(rset,runcontrolRSET);

static void ProcessCallback(CALLBACK *pcb)
{
    dbCommon *prec;

    callbackGetUser(prec, pcb);
    dbScanLock(prec);
    (*prec->rset->process)(prec);
    dbScanUnlock(prec);
}

static long init_record(runcontrolRecord *prec, int pass)
{
    if (pass==0) {
        CALLBACK *pcb = callocMustSucceed(1, sizeof(CALLBACK),
            "runcontrolRecord:init_record CALLBACK structure");

        callbackSetCallback(ProcessCallback, pcb);
        callbackSetUser(prec, pcb);
        callbackSetPriority(prec->prio, pcb);

        prec->dpvt = pcb;

        prec->lsem = 1;
        prec->areq = 0;
        prec->chbt = 5.0;
        prec->lalm = 0;
        prec->udf = FALSE;

        prec->alrm = MAJOR_ALARM;
        recGblSetSevr(prec, SOFT_ALARM, MAJOR_ALARM);
        recGblResetAlarms(prec);

        strcpy(prec->msg,"IOC rebooted");
    }
    return 0;
}

static long process(runcontrolRecord *prec)
{
    prec->pact = TRUE;
    if (prec->run || prec->areq) {
        /* Timeout. Client did not ping us frequently enough. Restore semaphore. */
        recGblSetSevr(prec, SOFT_ALARM, MAJOR_ALARM);
        if (prec->areq) {
            strncpy(prec->msg, "** Aborted **", MAX_STRING_SIZE);
            prec->areq = 0;
        } else {
            prec->hbto = 1;
            recGblGetTimeStamp(prec);
        }
        prec->run = 0;
    }
    monitor(prec);
    recGblFwdLink(prec);
    prec->pact = FALSE;

    return 0;
}

static long special(DBADDR *paddr, int after)
{
    runcontrolRecord *prec = (runcontrolRecord *)paddr->precord;
    CALLBACK *pcb = prec->dpvt;
    unsigned short monitor_mask;

    if (after) {
        if (paddr->pfield == &(prec->sem)) {
            if (prec->sem == 0  && prec->lsem == 1) {
                /* Successful semTake. Start watchdog and run indicator. */
                prec->lsem = 0;
                prec->chbt = prec->hbt / 1000.0;
                callbackRequestDelayed(pcb, prec->chbt);
                prec->abrt = 0;
                prec->susp = 0;
                prec->hbto = 0;
                prec->run = 1;
                prec->msg[0] = '\0';
                monitor_mask = recGblResetAlarms(prec);
                monitor_mask |= (DBE_LOG | DBE_VALUE);
                db_post_events(prec, &(prec->run), monitor_mask);
                db_post_events(prec, &(prec->abrt), monitor_mask);
                db_post_events(prec, &(prec->susp), monitor_mask);
                db_post_events(prec, &(prec->hbto), monitor_mask);
                db_post_events(prec, &(prec->msg), monitor_mask);
                prec->udf = FALSE;
                recGblGetTimeStamp(prec);
                return 0;
            } else if (prec->sem == 0  && prec->lsem == 0) {
                return 1;           /* failed semTake */
            } else if (prec->sem == 1  && prec->lsem == 0) {
                /* Client ending and doing a semGive. Cancel timer and reset. */
                epicsTimerCancel(pcb->timer);
                prec->areq = 0;

                reset(prec);
                return 0;
            } else { /* (prec->sem == 1  && prec->lsem == 1) */
                return 0;           /* extra semGive */
            }
        } else if (paddr->pfield == &(prec->val)) {
            /* We are being pinged by the client. Reset the watchdog. */
            if (prec->run == 0) {
                return 0;
            }
            if (prec->areq) {
                return 0;
            }
            epicsTimerCancel(pcb->timer);
            callbackRequestDelayed(pcb, prec->chbt);

            return 0;
        } else if (paddr->pfield == &(prec->abrt)) {
            /* abrt only acted on if semaphore is taken */
            if (prec->lsem == 0 && !prec->hbto) {
                epicsTimerCancel(pcb->timer);
                callbackRequestDelayed(pcb, prec->chbt);
                prec->areq = 1;

                recGblSetSevr(prec, SOFT_ALARM, MAJOR_ALARM);
                strncpy(prec->msg, "** Abort Requested **", MAX_STRING_SIZE);
                monitor(prec);
            } else {
                prec->abrt = 0;
            }

            return 0;
        } else if (paddr->pfield == &(prec->susp) &&
                   prec->lsem == 0 && !prec->abrt &&
                   !prec->hbto) { 
            if (prec->susp == 0) {  /* Resume */
                epicsTimerCancel(pcb->timer);
                callbackRequestDelayed(pcb, prec->chbt);
                prec->run = 1;

                /* Restore saved msg and alarm settings from suspend */
                prec->alrm = prec->lalm;
                strncpy(prec->msg, prec->lmsg, MAX_STRING_SIZE);
                recGblSetSevr(prec, SOFT_ALARM, prec->alrm);

                monitor_mask = recGblResetAlarms(prec);
                monitor_mask |= (DBE_LOG | DBE_VALUE);
                db_post_events(prec, &(prec->run), monitor_mask);
                db_post_events(prec, &(prec->msg), monitor_mask);
            } else {                       /* Suspend */
                epicsTimerCancel(pcb->timer);
                prec->run = 0;

                /* Save msg and alarm settings for resume */
                prec->lalm = prec->alrm;
                if (strcmp(prec->msg,"** Suspended **"))
                      strncpy(prec->lmsg, prec->msg, MAX_STRING_SIZE);
                strcpy(prec->msg, "** Suspended **");
                recGblSetSevr(prec, SOFT_ALARM, MAJOR_ALARM);
                monitor_mask = recGblResetAlarms(prec);
                monitor_mask |= (DBE_LOG | DBE_VALUE);
                db_post_events(prec, &(prec->run), monitor_mask);
                db_post_events(prec, &(prec->msg), monitor_mask);
            }
            return 0;
        } else if (paddr->pfield == &(prec->clr)) {
            if (!prec->areq) {
                /* Clear info fields after a timeout or abort. */
                if ((prec->hbto == 1 && prec->clr == 1) ||
                    prec->abrt) {
                    prec->hbto = 0;
                    reset(prec);
                }
            }
        } else if (paddr->pfield == &(prec->alrm)) {
            if (prec->run && !prec->susp && !prec->areq) {
                recGblSetSevr(prec, SOFT_ALARM, prec->alrm);
                monitor_mask = recGblResetAlarms(prec);
                monitor_mask |= (DBE_LOG | DBE_VALUE);
                db_post_events(prec, &(prec->run), monitor_mask);
                db_post_events(prec, &(prec->msg), monitor_mask);
            }
            /* re-assert this message, since user may have poked something
             * over it before abort is completed. */
            if (prec->areq) 
                strncpy(prec->msg, "** Abort Requested **", MAX_STRING_SIZE);
        }
    }
    return 0;
}

static long get_precision(DBADDR *paddr, long *precision) {
    *precision = 0;
    return 0;
}

static long get_enum_str(DBADDR *paddr, char *pstring)
{
    runcontrolRecord  *prec = (runcontrolRecord *)paddr->precord;

    if (paddr->pfield == &(prec->hbto)) {
        switch (prec->hbto) {
        case 0:
            strcpy(pstring, "Running");
            break;
        case 1:
            strcpy(pstring, "Timeout");
            break;
        default:
            strcpy(pstring, "Unknown");
        }
    } else if (paddr->pfield == &(prec->abrt)) {
        switch (prec->abrt) {
        case 0:
            strcpy(pstring, "Run");
            break;
        case 1:
            strcpy(pstring, "Abort");
            break;
        default:
            strcpy(pstring, "Unknown");
        }
    } else if (paddr->pfield == &(prec->susp)) {
        switch (prec->susp) {
        case 0:
            strcpy(pstring, "Resume");
            break;
        case 1:
            strcpy(pstring, "Suspend");
            break;
        default:
            strcpy(pstring, "Unknown");
        }
    }
    return 0;
}

static long put_enum_str(DBADDR *paddr, char *pstring)
{
    runcontrolRecord *prec = (runcontrolRecord *)paddr->precord;

    if (paddr->pfield == &(prec->abrt)) {
        if (strcmp(pstring,"Run") == 0)
            prec->abrt = 0;
        else if (strcmp(pstring,"Abort") == 0)
            prec->abrt = 1;
        else
            return S_db_badChoice;
    } else if (paddr->pfield == &(prec->susp)) {
        if (strcmp(pstring, "Resume") == 0)
            prec->susp = 0;
        else if (strcmp(pstring, "Suspend") == 0)
            prec->susp = 1;
        else
            return S_db_badChoice;
    }
    return 0;
}

static long get_enum_strs(DBADDR *paddr, struct dbr_enumStrs *pes)
{
    runcontrolRecord *prec = (runcontrolRecord *)paddr->precord;

    if (paddr->pfield == &(prec->hbto)) {
        pes->no_str = 2;
        memset(pes->strs, '\0', sizeof(pes->strs));
        strcpy(pes->strs[0], "Running");
        strcpy(pes->strs[1], "Timeout");
    } else if (paddr->pfield == &(prec->abrt)) {
        pes->no_str = 2;
        memset(pes->strs, '\0', sizeof(pes->strs));
        strcpy(pes->strs[0], "Run");
        strcpy(pes->strs[1], "Abort");
    } else if (paddr->pfield == &(prec->susp)) {
        pes->no_str = 2;
        memset(pes->strs, '\0', sizeof(pes->strs));
        strcpy(pes->strs[0], "Resume");
        strcpy(pes->strs[1], "Suspend");
    }
    return 0;
}

static void reset(runcontrolRecord *prec)
{
    prec->lsem = 1;
    prec->lmsg[0] = '\0';
    prec->lalm = 0;
    prec->run = 0;
    prec->abrt = 0;
    prec->desc[0] = '\0';
    prec->host[0] = '\0';
    prec->pid[0] = '\0';
    prec->user[0] = '\0';
    prec->strt[0] = '\0';
    prec->msg[0] = '\0';

    recGblSetSevr(prec, SOFT_ALARM, MAJOR_ALARM);
    monitor(prec);
}
static void monitor(runcontrolRecord *prec)
{
    unsigned int monitor_mask = recGblResetAlarms(prec);
    monitor_mask |= (DBE_LOG|DBE_VALUE);

    if (monitor_mask) {
        db_post_events(prec, &(prec->desc), monitor_mask);
        db_post_events(prec, &(prec->hbto), monitor_mask);
        db_post_events(prec, &(prec->pid), monitor_mask);
        db_post_events(prec, &(prec->strt), monitor_mask);
        db_post_events(prec, &(prec->host), monitor_mask);
        db_post_events(prec, &(prec->user), monitor_mask);
        db_post_events(prec, &(prec->susp), monitor_mask);
        db_post_events(prec, &(prec->abrt), monitor_mask);
        db_post_events(prec, &(prec->run), monitor_mask);
        db_post_events(prec, &(prec->msg), monitor_mask);
    }
    return;
}
