#ifndef __INCL_LYNTRA_SECURITY_H__
#define __INCL_LYNTRA_SECURITY_H__

#define AES_128 0x0001
#define AES_192 0x0002
#define AES_256 0x0003
				  
#define AES_ECB 0x0100
#define AES_CBC 0x0200
#define AES_GCM 0x0300
				  
#define TLS_1_2 0x0010
#define TLS_1_3 0x0020

#define CHACHA20 0x1000
#define POLY1305 0x2000

#endif //__INCL_LYNTRA_SECURITY_H__