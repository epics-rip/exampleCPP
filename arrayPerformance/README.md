<h1>arrayPerformance</h1>
<p>This example implements main programs that demonstrate performance
of large arrays and can also be used to check for memory leaks.
Checking for memory leaks can be accomplished by running the programs with valgrind
or some other memory check program.
</p>
<h2>Brief Summary</h2>
<p>The programs are:</p>
<dl>
   <dt>arrayPerformanceMain</dt>
     <dd>This is server and also a configurable number of longArrayMonitor clients.
     The clients can use either the local or
    remote providers. The monitor code is the same code that is used by longArrayMonitorMain.
     </dd>
   <dt>longArrayMonitorMain</dt>
     <dd>Remote client that monitors the array served by arrayPerformanceMain.</dd>
   <dt>longArrayGetMain</dt>
     <dd>Remote client that uses channelGet to access the array served by arrayPerformanceMain.</dd>
   <dt>longArrayPutMain</dt>
     <dd>Remote client that uses channelPut to access the array served by arrayPerformanceMain.</dd>
</dl>
<p>Each has support for <b>-help</b>.</p>
<pre>
mrk&gt; pwd
/home/hg/pvExampleTestCPP/arrayPerformance
mrk&gt; bin/linux-x86_64/arrayPerformanceMain -help
arrayPerformanceMain recordName size delay providerName nMonitor queueSize waitTime
default
arrayPerformanceMain arrayPerformance 10000000 0.0001 local 1 2 0.0

mrk&gt; bin/linux-x86_64/longArrayMonitorMain -help
longArrayMonitorMain channelName queueSize waitTime
default
longArrayMonitorMain arrayPerformance 2 0.0

mrk&gt; bin/linux-x86_64/longArrayGetMain -help
longArrayGetMain channelName iterBetweenCreateChannel iterBetweenCreateChannelGet delayTime
default
longArrayGetMain arrayPerformance 0 0 1

mrk&gt; bin/linux-x86_64/longArrayPutMain -help
longArrayPutMain channelName arraySize iterBetweenCreateChannel iterBetweenCreateChannelPut delayTime
default
longArrayPutMain arrayPerformance 10 0 0 1

mrk&gt; 
</pre>
<h2>Example output</h2>
<p><b>Note:</b> These may fail if run on a platform that does not have sufficient memory,</p>
<p>To see an example just execute the following commands in four different terminal windows:</p>
<pre>
bin/linux/&lt;arch&gt;/arrayPerformanceMain
bin/linux/&lt;arch&gt;/longArrayMonitorMain
bin/linux/&lt;arch&gt;/longArrayGetMain
bin/linux/&lt;arch&gt;/longArrayPutMain
</pre>
<p>Each program generates a report every second when it has something to report.
Examples are:</p>
<pre>
mrk&gt; bin/linux-x86_64/arrayPerformanceMain
arrayPerformance arrayPerformance 10000000 0.0001 local 1 2 0
...
 monitors/sec 66 first 131 last 131 changed {1, 2} overrun {} megaElements/sec 656.999
arrayPerformance value 132 time 1.00486 Iterations/sec 65.681 megaElements/sec 656.81
 monitors/sec 66 first 197 last 197 changed {1, 2} overrun {} megaElements/sec 656.304
arrayPerformance value 198 time 1.00563 Iterations/sec 65.6307 megaElements/sec 656.307
 monitors/sec 66 first 263 last 263 changed {1, 2} overrun {} megaElements/sec 654.824
...
</pre>
<pre>
mrk&gt; bin/linux-x86_64/longArrayMonitorMain
longArrayMonitorMain arrayPerformance 2 0
...
 monitors/sec 6 first 2357 last 2357 changed {1, 2} overrun {} megaElements/sec 68.6406
 monitors/sec 13 first 2385 last 2385 changed {1, 2} overrun {} megaElements/sec 118.72
 monitors/sec 9 first 2418 last 2418 changed {1, 2} overrun {1, 2} megaElements/sec 85.0984
...
</pre>
<pre>
mrk&gt; bin/linux-x86_64/longArrayPutMain
longArrayPutMain arrayPerformance 10 0 0 1
...
put numChannelPut 0 time 1.00148 Elements/sec 79.8819
put numChannelPut 1 time 1.00176 Elements/sec 79.8598
...
</pre>
<pre>
mrk&gt; bin/linux-x86_64/longArrayGetMain
longArrayGetMain arrayPerformance 0 0 1
...
get kiloElements/sec 7384.61
get kiloElements/sec 8726.34
...
</pre>

<h2>arrayPerformance</h2>
<p>The arguments for arrayPerforamanceMain are:</p> 
<dl>
  <dt>recordName</dt>
     <dd>The name for the arrayPerform record.</dd>
  <dt>size</dt>
     <dd>The size for the long array of the value field.</dd>
  <dt>delay</dt>
      <dd>The time in seconds to sleep after each iteration.</dd>
  <dt>providerName</dt>
      <dd>The name of the channel provider for the longArrayMonitors 
      created by the main program. This must be either <b>local</b>
      or <b>pvAccess</b>.
      </dd>
  <dt>nMonitor</dt>
     <dd>The number of longArrayMonitors to create.</dd>
  <dt>queueSize</dt>
     <dd>The queueSize for the element queue.
      A value less than 1 will become 1.
     </dd>
  <dt>waitTime</dt>
     <dd>The time that longArrayMonitor will sleep after poll returns a monitorElement.</dd>
</dl>
<p>
arrayPerformance creates a PVRecord that has the structure:.
</p>
<pre>
recordName
    long[] value
    timeStamp timeStamp
    alarm alarm
</pre>
Thus it holds an array of 64 bit integers.
<p>The record has support that consists of a separate thread that runs
until the record is destroyed executing the following algorithm:</p>
<dl>
  <dt>report</dt>
    <dd>Once a second it produces a report.
     In the above example output each line starting with
    <b>ArrayPerformance</b> is an arrayPerformance report.
    </dd>
  <dt>create array</dt>
     <dd>A new shared_vector is created and each element is set equal
      to the iteration count.</dd>
  <dt>lock</dt>
     <dd>The arrayPerformance record is locked.</dd>
  <dt>Begin group put</dt>
     <dd>beginGroupReport is called.</dd>
  <dt>replace</dt>
      <dd>The value field of the record is replaced
      with the newly created shared_vector.</dd>
  <dt>process</dt>
      <dd>The record is then processed. This causes the timeStamp to
       be set to the current time.</dd>
  <dt>End group put</dt>
    <dd>endGroupPut is called.</dd>
  <dt>unlock</dt>
     <dd>The arrayPerformance record is unlocked.</dd>
  <dt>delay</dt>
      <dd>If delay is greater than zero epicsThreadSleep is called.</dd>
</dl>
<h2>longArrayMonitor</h2>
<p>This is a pvAccess client that monitors an arrayPerformance record.
It generates a report every second showing how many elements has received.
For every monitor it also checks that the number of elements is &gt;0 and the
the first element equals the last element. It reports an error if either
of these conditions is not true.</p>
<p>The arguments for longArrayMonitorMain are:</p>
<dl>
  <dt>channelName</dt>
     <dd>The name for the arrayPerform record.</dd>
  <dt>queueSize</dt>
     <dd>The queueSize. Note that any size &lt;2 is made 2.</dd>
  <dt>waitTime</dt>
     <dd>The time to wait after a poll request returns a monitorElement.
     This can be used to force an overrun of the client even if there is no
     overrun on the server.</dd>
</dl>
<h2>longArrayGet</h2>
<p>This is a pvAccess client that uses channelGet to access an arrayPerformance record.
Every second it produces a report.</p>

<p>The arguments for longArrayGetMain are:</p>
<dl>
  <dt>channelName</dt>
     <dd>The name for the arrayPerform record.</dd>
  <dt>iterBetweenCreateChannel</dt>
      <dd>The number of iterations between destroying and recreating the channel.
      A value of 0 means never destroy and recreate.
      </dd>
  <dt>iterBetweenCreateChannelGet</dt>
      <dd>The number of iterations between destroying and recreating the channelGet.
      A value of 0 means never destroy and recreate.
      </dd>
  <dt>delayTime</dt>
     <dd>The time to delay between gets.</dd>
</dl>
<h2>longArrayPut</h2>
<p>This is a pvAccess client that uses channelPut to access an arrayPerformance record.
Every second it produces a report.</p>

<p>The arguments for longArrayPutMain are:</p>
<dl>
  <dt>channelName</dt>
     <dd>The name for the arrayPerform record.</dd>
  <dt>arraySize</dt>
     <dd>The capacity and length of the array to put to the server.</dd>
  <dt>iterBetweenCreateChannel</dt>
      <dd>The number of iterations between destroying and recreating the channel.
      A value of 0 means never destroy and recreate.
      </dd>
  <dt>iterBetweenCreateChannelPut</dt>
      <dd>The number of iterations between destroying and recreating the channelPut.
      A value of 0 means never destroy and recreate.
      </dd>
  <dt>delayTime</dt>
     <dd>The time to delay between gets.</dd>
</dl>

<h2>Some results</h2>
<h3>array performance</h3>
<p>The results were from my laptop in 2013
It had a 2.2Ghz intel core i7 with 4Gbytes of memory.
The operating system was linux fedora 16.</p>
<p>When test are performed with large arrays it is a good idea to also
run a system monitor facility and check memory and swap history.
If a test configuration causes physical memory to be exhausted
then performance becomes <b>very</b> poor.
You do not want to do this.</p>
<h3>arrayPerformance results</h3>
<p>The simplest test to run arrayPerformance with the defaults:</p>
<pre>
mrk&gt; pwd
/home/hg/pvExampleTestCPP/arrayPerformance
mrk&gt; bin/linux-x86_64/arrayPerformanceMain
</pre>
<p>This means that the array will hold 10 million elements.
The delay will be a millisecond.
There will be a single monitor and it will connect directly
to the local channelProvider, i. e. it will not use any network
connection.</p>
<p>The report shows that arrayPerformance can perform about 50 iterations per second
and is putting about 500million elements per second.
Since each element is an int64 this means about 4gigaBytes per second.
</p>
<p>When no monitors are requested and a remote longArrayMonitorMain is run:</p>
<pre>
mr&gt; pwd
/home/hg/pvExampleTestCPP/arrayPerformance
mrk&gt; bin/linux-x86_64/longArrayMonitorMain
</pre>
<p>The performance drops to about 25 iterations per second and 250 million elements per second.
The next section has an example that demonstrates what happens.
Note that if the array size is small enough to fit in the local cache then running longArrayMonitor
has almost no effect of arrayPerforance.
</p>
<h3>memory leaks</h3>
<p>Running longArrayMonitorMain, longArrayPutMain, and longArrayGetMain
under valgrind shows no memory leaks.</p>
<p>arrayPerformanceMain shows the following:</p>
<pre>
==9125== LEAK SUMMARY:
==9125==    definitely lost: 0 bytes in 0 blocks
==9125==    indirectly lost: 0 bytes in 0 blocks
==9125==      possibly lost: 576 bytes in 2 blocks
</pre>
<p>The possibly leaked is either 1 or 2 blocks.
It seems to be the same if clients are connected.
</p>

