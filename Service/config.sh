#!/usr/bin/env bash

global_path="`cd ..`"
exec_path="/usr/bin/telemetry_service/"

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
conf_path="/usr/bin/telemtry_service/telemetry.srvice"
EOF

cp "telemetry_${option}.service" "${exec_path}/${option}.service"
}


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

        systemctl daemon-reload

        read -p "Would want you to autorun telemtry service? (y/n or another key)" choice

        if [$choice=="y" or $choice=="Y"] then
            systemctl enable telemetry.service
        else break
        fi 

        systemctl start telemetry.service
        systemctl status telemetry.service       

    else break
    fi 
}


gen_ciphers(){
    cd "$(pwd)/cryptkeys"
    openssl genpkey -algorithm ed25519 -out ed25519-priv-key.pem
    openssl pkey -in ed25519key.pem -pubout
}


check_dependencies()
setup_service()
gen_ciphers()

exit 0;
