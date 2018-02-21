/*
 * dissectPackets.c
 * Created on Apr 28, 2015
 *     Author: Daniel Slapelis (djs7209)
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_PACKET_SIZE 2048

void printProt( int prot );

int main(int argc, char* argv[])
{
    /// if not enough arguments are given, program fails
    if(argc < 2)
    {
         fprintf(stderr, "usage: dissectPackets inputFile\n");
         return(EXIT_FAILURE);
    }else{

        /// otherwise we read the file
        FILE* file = fopen(argv[1], "rb");
        if(!file)
        {
            /// if the file cant be read the program quits
            fprintf(stderr, "Failed to open input file: No such file or directory\n");
            return(EXIT_FAILURE);
        }

        /// number of packets is first line of the file
        unsigned int* numPackets = malloc(sizeof(int));
        fread(numPackets, sizeof(int), 1, file);
        
 
        printf("==== File %s contains %u Packets.\n", argv[1], *numPackets);

        /// loop for printing the data of each packet
        for( unsigned int i = 0; i < *numPackets; i++ )
        {
            printf("==>Packet %u\n", (i+1));
        
            /// packet size in bytes
            unsigned int* packetSize = malloc(sizeof(int));
            fread(packetSize, sizeof(int), 1, file);

            /// first 20 bytes of the packet
            unsigned char* buffer = malloc(sizeof(char) * MAX_PACKET_SIZE);
            fread(buffer, sizeof(char), *packetSize, file);
            
            /// print the body of the packet
            int ver = buffer[0] >> 4;
            int ihl = buffer[0] & (0x0f);
            int tos = buffer[1];
            int totalLen = (buffer[2] << 8) + buffer[3];
            int ident = (buffer[4] << 8) + buffer[5];
            //printf("BUFFER 4: %d, BUFFER 5: %d\n", buffer[4], buffer[5]);
            int ipFlags = buffer[6] >> 3;
            int fragOff = (buffer[6] & (0x1F)) + buffer[7];
            int ttl = buffer[8]; 
            int prot = buffer[9];
            //printf("BUFFER 10: %d, BUFFER 11: %d\n", buffer[10], buffer[11]);
            int chksum = (buffer[10] << 8) + buffer[11];
            //int sourceAddress = buffer[11] + buffer[12] + buffer[13] + buffer[14];
            //int destAddress = buffer[15] + buffer[16] + buffer[17] + buffer[18];           
	    /// our formatted printing 
            printf("Version:		0x%X (%d)\n", ver, ver);
            printf("IHL (Header Length):		0x%X (%d)\n", ihl, ihl);
            printf("Type of Service (TOS):		0x%X (%d)\n", tos, tos);
            printf("Total Length:		0x%X (%d)\n", totalLen, totalLen);
            printf("Identification:		0x%X (%d)\n", ident, ident);
            printf("IP Flags:		0x%X (%d)\n", ipFlags, ipFlags);
            printf("Fragment Offset:		0x%X (%d)\n", fragOff, fragOff);
            printf("Time To Live (TTL):		0x%X (%d)\n", ttl, ttl);
            printProt(prot);
	    printf("Header Checksum:		0x%X (%d)\n", chksum, chksum);
            printf("Source Address:		%d.%d.%d.%d\n", buffer[12], buffer[13], 
                buffer[14], buffer[15]);
            printf("Destination Address:		%d.%d.%d.%d\n", buffer[16], 
		buffer[17], buffer[18], buffer[19]);
              
            /// freeing used memory
            free(packetSize);
            free(buffer);
        }
        fclose(file);
    }
    return(EXIT_SUCCESS);
} 

void printProt( int prot )
{
    if(prot == 1)
    {
        printf("Protocol:           ICMP 0x%X (%d)\n", prot, prot);
    }

    if(prot == 2)
    {
        printf("Protocol:           IGMP 0x%X (%d)\n", prot, prot);
    }
 
    if(prot == 6)
    {
        printf("Protocol:           TCP 0x%X (%d)\n", prot, prot);
    }

    if(prot == 9)
    {
        printf("Protocol:           IGRP 0x%X (%d)\n", prot, prot);
    }

    if(prot == 17)
    {
        printf("Protocol:           UDP 0x%X (%d)\n", prot, prot);
    }

    if(prot == 47)
    {
        printf("Protocol:           GRE 0x%X (%d)\n", prot, prot);
    }

    if(prot == 50)
    {
        printf("Protocol:           ESP 0x%X (%d)\n", prot, prot);
    }

    if(prot == 51)
    {
        printf("Protocol:           AH 0x%X (%d)\n", prot, prot);
    }

    if(prot == 57)
    {
        printf("Protocol:           SKIP 0x%X (%d)\n", prot, prot);
    }

    if(prot == 88)
    {
        printf("Protocol:           EIGRP 0x%X (%d)\n", prot, prot);
    }

    if(prot == 89)
    {
        printf("Protocol:           OSPF 0x%X (%d)\n", prot, prot);
    }

    if(prot == 115)
    {
        printf("Protocol:           L2TP 0x%X (%d)\n", prot, prot);
    }
}
