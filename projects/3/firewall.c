#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pktUtility.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "usage: firewall inputFile\n");
        return(EXIT_FAILURE);
    }else{
        printf("%s\n", argv[0]);
    }
}

/// Reads the source IP address from the IP header of the
/// packet. In the header the source IP address is formatted
/// as 4 bytes in big endian byte order. The bytes are shifted
/// and packed into an unsigned int.
/// @param pkt The packet to examine
/// @return The IP address of the computer that sent the packet
unsigned int ExtractSrcAddrFromIpHeader(unsigned char* pkt)
{
    unsigned int src = (pkt[12] << 8) + (pkt[13] << 8) + 
                       (pkt[14] << 8) + (pkt[15]);
    return src;
}

/// Reads the destination IP address from the IP header of the
/// packet. In the header the destination IP address is formatted
/// as 4 bytes in big endian byte order. The bytes are shifted
/// and packed into an unsigned int.
/// @param pkt The packet to examine
/// @return The IP address of the computer that the packet
/// is addressed to
unsigned int ExtractDstAddrFromIpHeader(unsigned char* pkt)
{
    unsigned int dst = (pkt[16] << 8) + (pkt[17] << 8) +
                       (pkt[18] << 8) + (pkt[19]);
    return dst;
}

/// Reads the protocol number from the IP header
/// @param pkt The packet to examine
/// @return The value stored in the protocol field of the IP header
unsigned int ExtractIpProtocol(unsigned char* pkt)
{
    unsigned int prot = pkt[9];
    return prot;
}

/// Reads the value of the Type field in the ICMP header
/// of an ICMP message. This function assumes that the ICMP
/// message is contained in an IP packet (with a standard
/// length 20 byte IP header)
/// @param pkt The packet to examine
/// @return The ICMP Type
unsigned char ExtractIcmpType(unsigned char* pkt)
{

    unsigned char typeOfService = pkt[1];
    printf("EXTRACTED ICMP TYPE: %u\n", typeOfService);
    return typeOfService;
}

/// Reads the destination port number out of the TCP header
/// of an IP packet containing a TCP protocol data unit. This
/// function assumes that the IP packet starts with a standard
/// length 20 byte IP header.
/// @param pkt The packet to examine
/// @return The destination port of the TCP protocol data unit
unsigned int ExtractTcpDstPort(unsigned char* pkt)
{
    unsigned int dstPort = (pkt[20] << 8) + pkt[21];
    return dstPort;
}

/// Converts an IP address represented as an array of 4 octets
/// into a single unsigned int
/// @details This function expects a pointer to the start of a unsigned
/// char array holding the IP address octets, for example: | C0 | A8 | 01 | 64 |
/// is converted into an unsigned int with value C0A80164 and returned
/// @param ip The IP address represented as an array of 4 octets
/// with each octet stored in an unsigned char
/// @return The IP address packed into an unsigned int
unsigned int ConvertIpUCharOctetsToUInt(unsigned char* ip)
{
    unsigned int converted = (unsigned int) ip[0] + 
				(unsigned int) ip[1] + 
				(unsigned int) ip[2] + 
				(unsigned int) ip[3];
    return converted;
}

/// Converts an IP address represented as an array of 4 octets
/// into a single unsigned int
/// @details This function expects a pointer to the start of a unsigned
/// int array holding the IP address octets, for example: | 000000C0 |
/// 000000A8 | 00000001 | 00000064 | is converted into an unsigned int
/// with value C0A80164 and returned
/// @param ip The IP address represented as an array of 4 octets
/// with each octet stored in an unsigned char
/// @return The IP address packed into an unsigned int
unsigned int ConvertIpUIntOctetsToUInt(unsigned int* ip)
{
    unsigned int converted = (ip[0] << 8) + (ip[1] << 8) + (ip[2] << 8) + ip[3];
    return converted;
}
