### 20190607
* change DEPEX for <em>CdePkg</em>Driver from gCdeCommandLineProtocolGuid to gCdeDxeProtocolGuid/CDE_DXE_PROTOCOL_GUID 
  since <em>CdeServices</em> already depends on gCdeCommandLineProtocolGuid.
  
  The sample implementation on the MinnowBoard is okay despite that DEPEX fault.
