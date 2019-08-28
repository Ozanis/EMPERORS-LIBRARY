#include "linkedlist.h"


class Handler : public LinkedList{
    public:
        Handler();
        ~Handler();
        void add_connection();
        void del_connection();
        void is_alive();
    private:
        size_t connectors = 0, cur_num = 0;
}


void Handler :: is_alive(){
    size_t set = 0;
    if(FD_ISSET(this->guest_port[i], handle_read)) ++set;
    else{
        if(!this->cur_num) break;
        else --this->cur_num;
        this->clients[i]->alive = false;
        try{
             shutdown(this->clients[i]->sock, 2);
             close(this->clients[i]->sock);
             memset(&this->clients[i]->sock, 0, sizeof(int));
             }
        throw exception("Socket have closed already");
     }
}


void Server add_connection(){
     if(this->cur_num >= this->num_of_clients)
        Cerr << "Connectors num overflow";
        return;
     int newsockfd = accept(this->sockfd, (sockaddr*)&this->clients->clientAddress, (socklen_t*)&    clients->this->len);
     if(this->newsockfd < 0){
         cerr << "Acception error";
         return;
     }
     for(size_t i = 0; i < this->num_of_client; i++){
         if(!this->clients[i]->alive){
             this->clients[i]->alive = true;
             this->clients[i]->sock = newsockfd;
         }
     }
     ++this->cur_num;
 }


void Handler :: del_connection(){

}


Handler :: ~Handler(){


}
