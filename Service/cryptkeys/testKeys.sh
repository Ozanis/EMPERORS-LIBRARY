openssl ecparam -out ecparam.pem -name prime256v1
openssl genpkey -paramfile ecparam.pem -out ecdhkey.pem
openssl req -x509 -new -key ecdhkey.pem -out cert.pem
