// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleHelloRPC.h */
/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLEHELLORPC_H
#define EXAMPLEHELLORPC_H

#ifdef epicsExportSharedSymbols
#   define exampleHelloRPCEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/rpcServer.h>

#ifdef exampleHelloRPCEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleHelloRPCEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace database {


class ExampleHelloRPC;
typedef std::tr1::shared_ptr<ExampleHelloRPC> ExampleHelloRPCPtr;


class epicsShareClass ExampleHelloRPC :
    public epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ExampleHelloRPC);
    static  epics::pvAccess::RPCService::shared_pointer create();
    epics::pvData::PVStructurePtr request(
         epics::pvData::PVStructurePtr const & pvArgument)
         throw (epics::pvAccess::RPCRequestException);
private :
    ExampleHelloRPC(epics::pvData::PVStructurePtr const & pvResult);
    epics::pvData::PVStructurePtr pvResult;
};

}}}

#endif  /* EXAMPLEHELLORPC_H */
