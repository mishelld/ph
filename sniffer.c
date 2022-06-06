#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>

struct ethheader {
  unsigned char  ether_dhost[ETHER_ADDR_LEN]; /* destination host address */
  unsigned char  ether_shost[ETHER_ADDR_LEN]; /* source host address */
  unsigned short ether_type;                  /* IP? ARP? RARP? etc */
};
void print(unsigned char* packet)
{
 struct ethheader *eth = (struct ethheader*)packet;
    if (ntohs(eth->ether_type)== 0x0800) {
        // ip header
        struct iphdr *ip = (struct iphdr *)(packet + sizeof(struct ethheader));
     
        int ip_hdr_len = ip->ihl*4;
        // icmp header
        struct icmphdr *icmph = (struct icmphdr *)(packet + sizeof(struct ethheader) + ip_hdr_len);
        //check if the ip protocol is ICMP
        if(ip->protocol== IPPROTO_ICMP) {
        //copy into a buffer the src and dest info, and trans to readble info
        struct sockaddr_in src,dest;
        memset(&src, 0, sizeof(src));
        src.sin_addr.s_addr = ip->saddr;
        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = ip->daddr;
        //print the src,dest,type and code
    printf("Source IP: %s\n",inet_ntoa(src.sin_addr));
    printf("Destination IP: %s\n",inet_ntoa(dest.sin_addr));
    printf("type %d\n",icmph->type);
    printf("code %d\n",icmph->code);
     }
    }   
}


int main() {
    char buffer[IP_MAXPACKET];
    struct sockaddr saddr;
    struct packet_mreq mr;

    // Create the raw socket
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  

    // Turn on the promiscuous mode. 
    mr.mr_type = PACKET_MR_PROMISC;                           
    setsockopt(sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr,  sizeof(mr));

    // Getting captured packets
    while (1) {
        int data_size=recvfrom(sock, buffer, IP_MAXPACKET, 0,   &saddr, (socklen_t*)sizeof(saddr));
        if(data_size){
        print(buffer);
        }
    

        

    }

    close(sock);
    return 0;
}

