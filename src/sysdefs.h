#ifndef SYSDEFS_H
#define SYSDEFS_H

#define __R volatile const
#define __W volatile
#define __RW volatile

typedef 	unsigned int 		uint32_t;
typedef 	unsigned short 		uint16_t;
typedef 	unsigned char 		uint8_t;
typedef 	__RW uint32_t 		registro_t;  //!< defino un tipo 'registro'.

#endif /* SYSDEFS_H */
