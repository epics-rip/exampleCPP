
<h1>exampleLink</h1>
<p>This example implements a PVRecord that accesses another PVRecord.
The following are discussed:</p>
<ul>
   <li>Shows how to build and start the example.</li>
   <li>Provide a discussion of accessing data via pvAccess</li>
   <li>Gives a brief description of an example that gets data for an array of doubles.</li>
</ul>

<h2>Building</h2>
<p>If a proper RELEASE.local file exists one or two directory levels above <b>exampleLink</b>
then just type:</p>
<pre>
make
</pre>
<p>
It can also be built by</p>
<pre>
cp configure/ExampleRELEASE.local configure/RELEASE.local
edit configure/RELEASE.local
make
</pre>

<h2>To start the exampleLink as part of a V3 IOC</h2>
<pre>
mrk&gt; pwd
/home/epicsv4/master/exampleCPP/exampleLink/iocBoot/exampleLink
mrk&gt; ../../bin/linux-x86_64/exampleLink st.local
</pre>
or
<pre>
mrk&gt; pwd
/home/epicsv4/master/exampleCPP/exampleLink/iocBoot/exampleLink
mrk&gt; ../../bin/linux-x86_64/exampleLink st.remote
</pre>

<h2>Discussion</h2>
<h3>Access Alternatives</h3>
<p>The process routine of a PVRecord can access other PVRecords in two ways:</p>
<dl>
  <dt>Directly accessing local pvDatabase</dt>
    <dd>
    If the other PVRecord is accessed via the master PVDatabase then
    threading issues are up to the implementation.
    For now this method will not be discussed.</dd>
  <dt>Access via pvAccess</dt>
    <dd>
      If access is via pvAccess then locking is handled by pvAccess.</dd>
</dl>
<p>Access via pvAccess can be done either by local or remote channel provider.</p>
<dl>
  <dt>Access via channelProviderLocal</dt>
    <dd>
    If the local pvAccess server is used the implementation must be careful that it does not
    cause deadlocks.
    When the process method is called the pvRecord for the process method is locked.
    When it makes a pvAccess get, put, etc request the other record is locked.
    Thus if a set of pvAccess links are implemented the possibility of deadlocks
    exists. A simple example is two records that have links to each other.
    More complex sets are easily created.
    Unless the developer has complete control of the set of records then remote pvAccess should
    be used.
    But this results in more context switches.
    </dd>
   <dt>Access via remote pvAccess</dt>
      <dd>If remote pvAccess is used then all locking issues are handled by pvAccess.
      The linked channel can be a pvRecord in the local pvDatabase or can be implemented
      by a remote pvAccess server.</dd>
</dl>
<h3>Data synchronization</h3>
<p>If pvAccess is used then it handles data synchronization.
This is done by making a copy of the data that is transferred between the two pvRecords.
This is true if either remote or local pvAccess is used.
Each get, put, etc request results in data being copied between the two records.</p>
<p>
If the linked channel is a local pvRecord then,
for scalar and structure arrays,
raw data is NOT copied for gets.
This is because pvData uses shared_vector to hold the raw data.
Instead of copying the raw data the reference count is incremented.</p>
<p>For puts the linked array will force a new allocation of the raw data in the linked record,
i. e. copy on write semantics are enforced. This is done automatically
by pvData and not by pvDatabase.</p>
<h3>Some details</h3>
<p>As mentioned before a pvDatabase server can be either a separate process,
i. e. a main program, or can be part of a V3IOC.</p>
<p>A main pvDatabase server issues the following calls:</p>
<pre>
...
    ContextLocal::shared_pointer contextLocal = ContextLocal::create();
    contextLocal-&gt;start();
...
    contextLocal-&gt;waitForExit();
</pre>
<p>The first call is only necessary if some of the pvRecords
have pvAccess links.
These must be called before any code that uses links is initialized.
After these two calls there will be two channel providers: <b>local</b>, and <b>pvAccess</b>.

</p>
<p>A pvDatabase that is part of a V3IOC has the following in the st.cmd file.</p>
<pre>
...
iocInit()
startPVAClient
startPVAServer
## commands to create pvRecords
</pre>
<p>
Once the client and local provider code has started then the following creates a channel access link.
</p>
<pre>
PVDatabasePtr master = PVDatabase::getMaster();
ChannelProvider::shared_pointer provider =
     getChannelProviderRegistry()-&gt;getProvider(providerName);
Channel::shared_pointer channel = provider-&gt;createChannel(channelName,channelRequester);
</pre>

<h2>Directory Layout</h2>
<pre>
exampleLink
    configure
       ExampleRELEASE.local
       ...
    src
       exampleLink.h
       exampleLink.cpp
       exampleLinkInclude.dbd
       exampleLinkRegister.cpp
    ioc
       Db
       src
          exampleLinkInclude.dbd
          exampleLinkMain.cpp
   iocBoot
      exampleLink
         st.local
         st.remote
         ...
</pre>
<p>This example is only built to be run as part of a V3 IOC.
Note that two startup files are available: st.local and st.remote.
st.local has two records: doubleArray and exampleLink.
doubleArray is a record that can be changed via a call to pvput.
exampleLink is a record that, when processed, gets the value from doubleArray and sets its value equal
to the value read.
st.local has both records.
st.remote has only one record named exampleLinkRemote.
</p>
<p>To start the example:</p>
<pre>
mrk&gt; pwd
/home/hg/pvExampleTestCPP/exampleLink/iocBoot/exampleLink
mrk&gt; ../../bin/linux-x86_64/exampleLink st.local 
</pre>
<p>then in another window:</p>
<pre>
mrk&gt; pvput doubleArray 4 100 200 300 400
Old : doubleArray 0
New : doubleArray 4 100 200 300 400
mrk&gt; pvget -r "record[process=true]field(value)" exampleLink
exampleLink
structure 
    double[] value [100,200,300,400]
mrk&gt; 
</pre>
<h2>exampleLink Implementation</h2>
<p>exampleLink.h contains the following:</p>
<pre>
...
class ExampleLink :
    public PVRecord,
    public epics::pvAccess::ChannelRequester,
    public epics::pvAccess::ChannelGetRequester
{
public:
    POINTER_DEFINITIONS(ExampleLink);
    static ExampleLinkPtr create(
        std::string const &amp; recordName,
        std::string const &amp; providerName,
        std::string const &amp; channelName
        );
    virtual ~ExampleLink() {}
    virtual void destroy();
    virtual bool init();
    virtual void process();
    virtual void channelCreated(
        const epics::pvData::Status&amp; status,
        epics::pvAccess::Channel::shared_pointer const &amp; channel);
    virtual void channelStateChange(
        epics::pvAccess::Channel::shared_pointer const &amp; channel,
        epics::pvAccess::Channel::ConnectionState connectionState);
    virtual void channelGetConnect(
        const epics::pvData::Status&amp; status,
        epics::pvAccess::ChannelGet::shared_pointer const &amp; channelGet,
        epics::pvData::PVStructure::shared_pointer const &amp; pvStructure,
        epics::pvData::BitSet::shared_pointer const &amp; bitSet);
    virtual void getDone(const epics::pvData::Status&amp; status);
private:
...
</pre>
<p>All the non-static methods are either PVRecord, PVChannel, or PVChannelGet methods
and will not be discussed further.
The create method is called to create a new PVRecord instance with code that will issue
a ChannelGet::get request every time the process method of the instance is called.
Some other pvAccess client can issue a channelGet, to the record instance, with a request
to process in order to test the example.</p>
<p>All of the initialization is done by a combination of the create and init methods so
lets look at them:</p>
<pre>
ExampleLinkPtr ExampleLink::create(
    String const &amp; recordName,
    String const &amp; providerName,
    String const &amp; channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()-&gt;scalarArray(
        pvDouble,"alarm.timeStamp");
    ExampleLinkPtr pvRecord(
        new ExampleLink(
           recordName,providerName,channelName,pvStructure));
    if(!pvRecord-&gt;init()) pvRecord.reset();
    return pvRecord;
}
</pre>
<p>This first creates a new ExampleLink instance,
and then calls the init method and the returns a ExampleLinkPtr.
Note that if init returns false it returns a pointer to NULL.</p>
<p>The init method is:</p>
<pre>
bool ExampleLink::init()
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()-&gt;getPVStructure();
    pvTimeStamp.attach(pvStructure-&gt;getSubField("timeStamp"));
    pvAlarm.attach(pvStructure-&gt;getSubField("alarm"));
    pvValue = static_pointer_cast&lt;PVDoubleArray&gt;(
        pvStructure-&gt;getScalarArrayField("value",pvDouble));
    if(pvValue==NULL) {
        return false;
    }
    ChannelAccess::shared_pointer channelAccess = getChannelAccess();
    ChannelProvider::shared_pointer provider =
        channelAccess-&gt;getProvider(providerName);
    if(provider==NULL) {
         cout &lt;&lt; getRecordName() &lt;&lt; " provider "
              &lt;&lt; providerName &lt;&lt; " does not exist" &lt;&lt; endl;
        return false;
    }
    ChannelRequester::shared_pointer channelRequester =
        dynamic_pointer_cast&lt;ChannelRequester&gt;(getPtrSelf());
    channel = provider-&gt;createChannel(channelName,channelRequester);
    event.wait();
    if(!status.isOK()) {
        cout &lt;&lt; getRecordName() &lt;&lt; " createChannel failed "
             &lt;&lt; status.getMessage() &lt;&lt; endl;
        return false;
    }
    ChannelGetRequester::shared_pointer channelGetRequester =
        dynamic_pointer_cast&lt;ChannelGetRequester&gt;(getPtrSelf());
    PVStructurePtr pvRequest = getCreateRequest()-&gt;createRequest(
        "value,alarm,timeStamp",getPtrSelf());
    channelGet = channel-&gt;createChannelGet(channelGetRequester,pvRequest);
    event.wait();
    if(!status.isOK()) {
        cout &lt;&lt; getRecordName() &lt;&lt; " createChannelGet failed "
             &lt;&lt; status.getMessage() &lt;&lt; endl;
        return false;
    }
    getPVValue = static_pointer_cast&lt;PVDoubleArray&gt;(
        getPVStructure-&gt;getScalarArrayField("value",pvDouble));
    if(getPVValue==NULL) {
        cout &lt;&lt; getRecordName() &lt;&lt; " get value not  PVDoubleArray" &lt;&lt; endl;
        return false;
    }
    return true;
}
</pre>
<p>This first makes sure the pvStructure has the fields it requires:</p>
<dl>
  <dt>timeStamp</dt>
    <dd>A timeStamp structure. This will be set to the current time when process is called.</dd>
  <dt>alarm</dt>
    <dd>An alarm structure. This will be used to pass status information to the client when
      process is called.</dd>
  <dt>value</dt>
     <dd>This must be a scalarArray of type double.
       It is where data is copied when the channelGet is issued.</dd>
</dl>
<p>Next it makes sure the channelProvider exists.</p>
<p>Next it creates the channel and waits until it connects.</p>
<p>Next it creates the channelGet and waits until it is created.</p>
<p>Next it makes sure it has connected to a double array field.</p>
<p>If anything goes wrong during initialization it returns false.
This a return of true means that it has successfully created a channelGet and is ready
to issue gets when process is called.</p>
<p>Look at the code for more details.</p>


