#include <pcap.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
  struct ethhdr *eth = (struct ethhdr *)packet;
  if (ntohs(eth->h_proto) == 0x0800) { // 0x0800 is IP type
    struct iphdr * ip = (struct iphdr *)(packet + sizeof(struct ethhdr)); 
    if(ip->protocol == IPPROTO_ICMP)
    {
      unsigned short iphdrlen;	
      struct iphdr *iph = (struct iphdr *)(packet  + sizeof(struct ethhdr));
      iphdrlen = iph->ihl * 4;      
      struct icmphdr *icmph = (struct icmphdr *)(packet + iphdrlen  + sizeof(struct ethhdr));
      struct sockaddr_in source,dest;

      memset(&source, 0, sizeof(source));
      source.sin_addr.s_addr = iph->saddr;
      
      memset(&dest, 0, sizeof(dest));
      dest.sin_addr.s_addr = iph->daddr;
	
      printf("\tFrom: %s\n", inet_ntoa(source.sin_addr));  
      printf("\tTo: %s\n", inet_ntoa(dest.sin_addr));   
      printf("\tProtocol: ICMP\n");
      printf("\tType: %d\n", ip->tos);
      printf("\tCode: %d\n", icmph->code);
      printf("\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }
  }
}

int main()
{
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program fp;
  char filter_exp[] = "ip proto icmp";
  bpf_u_int32 net;
  // Step 1: Open live pcap session on NIC with name eth3
  handle = pcap_open_live("enp0s3", BUFSIZ, 1, 1000, errbuf); 

  // Step 2: Compile filter_exp into BPF psuedo-code
  pcap_compile(handle, &fp, filter_exp, 0, net);      

  pcap_setfilter(handle, &fp);  
  printf("Ready to capture packets\n");         


  // Step 3: Capture packets
  pcap_loop(handle, -1, got_packet, NULL);                

  pcap_close(handle);   //Close the handle 
  return 0;
}