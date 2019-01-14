# Fill Downloader #

Using the TT REST API, this app will continuously download fills over a given time span per day.

The output is a comma delimited csv file, where each line represents an individual fill.  This mimcs the Export logic of the Fills widget in the TT platform.

When the application is executed, an app_secret will be requested.  Please follow the instructions [here](https://library.tradingtechnologies.com/tt-rest/gs-getting_started_with_tt-rest.html) to obtain an app_key and app_secret.

The user supplied start and end times are in 24 hour format and follow the UTC time zone.
The valid TT environments which can be accessed are ext-prod-live and ext-uat-cert.
The rate parameter represents how often to fetch fills from the servers, default is every minute.

