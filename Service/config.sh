#!/usr/bin/env bash

if [["`whoami`" -ne "root"]] then
    echo "You are not root. Please retry."
    exit 0
fi


global_path="`cd ..`"
exec_path="/usr/bin/telemetry_service/"


check_dependencies(){
    if[[eval "pacman -Qs openssl" -eq ""]] pacman -Sy openssl
}


load_binary(option){
    cur_path="${global_path}/${option}"
    mkdir "${cur_path}"
    cd "${cur_path}"
    cmake -DCMAKE_BUILD_TYPE=Release
    make
    cp "${option}" "${exec_path}/${option}"
    cd "${global_path}"
}


setup_service(){
    read -p "Do you watn to run script as systemd-service? (y/n or another key to reject)" choice

    if [$choice=="y" or $choice=="Y"] then
        while true; do
            read -p "Which config may I setup: Server or Client? (S/C for appropriate option)" choice
            case $choice in
                "S"|"s") load_binary("Server") && create_conf("Server");;
                "C"|"c") load_binary("Client") && create_conf("Client");;
                *)
                 echo "Wrong option: please repeat your choice or cancel"
                 continue;;
            esac
        done
    fi
}
 

create_conf(option){
    cat > "telemetry_${option}.service" <<EOF
[Unit]
Description="Telemetry service"
After=multi-user.target
Conflicts=getty@tty1.service

[Service]
Type=simple
ExecStart=/usr/bin/telemetry_service/telemetry_${option}
StandardInput=tty-force
Restart=always

[Install]
WantedBy=multi-user.target
exec_path="`cd ..`"
conf_path="/usr/bin/telemtry_service/telemetry_${option}.srvice"
EOF

    cp "telemetry_${option}.service" "${exec_path}/telemetry_${option}.service"

    read -p "Would want you to autorun telemtry service? (y/n or another key)" choice

    if [$choice=="y" or $choice=="Y"] then
        systemctl enable telemetry.service
            else break
            fi 

    read -p "Should service start now? (y/n or another key)" choice

    if [$choice=="y" or $choice=="Y"] then

         systemctl start telemetry_${option}.service
         systemctl status telemetry_${option}.service       
    else 
         echo "You always can do that using next command:\"systemctl start telemetry_${option}.service\""
    fi 
        systemctl daemon-reload
}


gen_ciphers(){
    cd "$(pwd)/cryptkeys"
    openssl ecparam -name c2tnb191v3 -out X9_62.pem
    openssl ecparam -in X9_62.pem -genkey -noout -out X9_62-key.pem
    openssl ecparam -in X9_62.pem -text -param_enc explicit -noout

    openssl req -x509 -new -SHA384 -nodes -key X9_62-key.pem 3650 -out X9_62-cert.pem
    openssl x509 -req -SHA384 -extfile v3.ext -days 365 -in X9_62-cert.pem -CA ca.crt -CAkey X9_62-key.pem -CAcreateserial -out X9_62-cert.pem
    openssl req -in X9_62-cert.pem -noout -text

    openssl ec -in X9_62-key.pem -pubout -out X9_62-pub.pem

#openssl genpkey -algorithm ed25519 -out ed25519-priv-key.pem
#openssl pkey -in ed25519key.pem -pubout
#    read -p "Few details are necessary for certificate creating. Please enter CN: " CN
#    read -p "Also I need cert owner email: " email
#    read -p "And last enter your email (example@service.com): " email
#    cat >> "${exec_path}/cryptkeys/openssl.cnf"<< EOF
#[ req ]
#prompt = no
#encrypt_key = no
#default_md = sha512
#distinguished_name = dname
#req_extensions = reqext

#[ dname ]
#CN = ${CN}
#emailAddress = ${email}
#
#[ reqext ]
#subjectAltName = ${altname}
#EOF
#    openssl req -new -config openssl.cnf -key privkey.pem -out csr.pem 
#    openssl req -in csr.pem -noout -text -verify
}


check_dependencies()
setup_service()
gen_ciphers()

exit 0;
