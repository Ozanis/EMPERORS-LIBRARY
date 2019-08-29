#!/usr/bin/env bash

setup_service(){
    read -p "Do you watn to run script as systemd-service? (y/n or another key)" choice

    if [$choice=="y" or $choice=="Y"] then
        setup_service()
    else break
    fi 

    path="/usr/bin/telemtry/telemetry.srvice"
    mkdir $path
    cp telemetry.service $path

    systemctl daemon-reload

    read -p "Would you to autorun telemtry service? (y/n or another key)" choice

    if [$choice=="y" or $choice=="Y"] then
        systemctl enable telemetry.service
    else break
    fi 

    systemctl start telemetry.service
    systemctl status telemetry.service       
}


create_ciphers(){
    cd "$(pwd)/cryptkeys"

    openssl genrsa -out key.pem 2048
    openssl req -new -key key.pem -out csr.pem -sha512
    openssl req -nodes -newkey rsa:2048 -keyout key.pem -out csr.pem  -sha512
    openssl req -new -x509  -days 365 -key key.pem  -out crt.pem -sha512

    openssl x509 -noout -modulus -in crt.pem | openssl md5
    openssl x509 -noout -modulus -in key.pem | openssl md5
    openssl x509 -noout -modulus -in csr.pem | openssl md5
}


create_ciphers()
service_setup()

