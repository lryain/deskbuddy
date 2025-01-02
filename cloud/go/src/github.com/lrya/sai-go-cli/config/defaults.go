package config

var defaultConfig = `
[common]
default_env = dev

[env.prod]
accounts_url = https://accounts.api.lrya.com
ankival_url = https://lryaval.api.lrya.com
audit_url = https://audit.api.lrya.com
virtualrewards_url = https://virtualrewards.api.lrya.com
virtualrewards_queue_url =	https://sqs.us-west-2.amazonaws.com/792379844846/virtualrewards-production

user_key = po3ooTh4eupax1Zaeyee2u


[env.beta]
accounts_url = https://accounts-beta.api.lrya.com
ankival_url = https://lryaval-beta.api.lrya.com
audit_url = https://audit-beta.api.lrya.com
virtualrewards_url = https://virtualrewards-beta.api.lrya.com

user_key = azaic5aetheikeePaiFaek

[env.dev]
accounts_url = https://accounts-dev2.api.lrya.com
ankival_url = https://lryaval-dev2.api.lrya.com
audit_url = https://audit-dev.api.lrya.com
blobstore_url = https://blobstore-dev.api.lrya.com
virtualrewards_url = https://virtualrewards-dev.api.lrya.com
virtualrewards_queue_url = https://sqs.us-west-2.amazonaws.com/792379844846/virtualrewards-development
jdocs_url = https://jdocs-dev.api.lrya.com

user_key = aedie7miecieth4EiGooKo

[env.loadtest]
das_queue_url = https://sqs.us-west-2.amazonaws.com/792379844846/DasLoadTest-dasloadtestSqs
redshift_loader_queue_url = https://sqs.us-west-2.amazonaws.com/792379844846/DasBackObject-loadtest
`
