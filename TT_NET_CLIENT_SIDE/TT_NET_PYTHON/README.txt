A number of people have been asking if TT will provide a Python wrapper for TT .NET SDK Client Side. This is not actually necessary since Iron Python (ironpython.net) already supports the ability to import and utilize any .NET DLL. And since TT .NET SDK Client Side includes a .NET DLL, you can take advantage of its capabilities in Python today.

As detailed here, the TT .NET SDK Client Side DLL can be obtained from NuGet via Visual Studio or downloaded manually. After you download it, make sure to keep abreast of subsequent releases by subscribing for automated email notifications when new versions of TT .NET SDK Client Side are released by clicking the 'Subscribe' button on the TT Release Notes web page.

https://labs.tradingtechnologies.com/apis/tt-net-sdk/gs-reference-dll.html
https://www.nuget.org/packages?q=TT.NET.SDK.ClientSide
https://library.tradingtechnologies.com/release_notes/production-2019-06.html

Once you've downloaded it, you can then import it in your Python code as follows:

import clr
clr.AddReferenceToFileAndPath("tt-net-api.dll")
from tt_net_sdk import *

And you're ready to go! I've written a simple sample (see attached) to demonstrate how I was able to call into the TT .NET SDK Client Side functionality. It submits an order that follows the bid. This should give you a good idea of how this works.

All of this syntax is defined in the Iron Python documentation. It's very well written. And I would highly suggest that you refer to it early and often if you plan to do this.

Cheers,
Andrew