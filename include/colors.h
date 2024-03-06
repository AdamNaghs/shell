/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 */
#ifndef COLORS_H
#define COLORS_H

//#define COLORS_ENABLED

#ifdef COLORS_ENABLED

/*Regular text*/
#ifndef BLK
#define BLK "\e[0;30m"
#endif
#ifndef RED
#define RED "\e[0;31m"
#endif
#ifndef GRN
#define GRN "\e[0;32m"
#endif
#ifndef YEL
#define YEL "\e[0;33m"
#endif
#ifndef BLU
#define BLU "\e[0;34m"
#endif
#ifndef MAG
#define MAG "\e[0;35m"
#endif
#ifndef CYN
#define CYN "\e[0;36m"
#endif
#ifndef WHT
#define WHT "\e[0;37m"
#endif 

/*Regular bold text*/
#ifndef BBLK
#define BBLK "\e[1;30m"
#endif
#ifndef BRED
#define BRED "\e[1;31m"
#endif
#ifndef BGRN
#define BGRN "\e[1;32m"
#endif
#ifndef BYEL
#define BYEL "\e[1;33m"
#endif
#ifndef BBLU
#define BBLU "\e[1;34m"
#endif
#ifndef BMAG
#define BMAG "\e[1;35m"
#endif
#ifndef BCYN
#define BCYN "\e[1;36m"
#endif
#ifndef BWHT
#define BWHT "\e[1;37m"
#endif

/*Regular underline text*/
#ifndef UBLK
#define UBLK "\e[4;30m"
#endif
#ifndef URED
#define URED "\e[4;31m"
#endif
#ifndef UGRN
#define UGRN "\e[4;32m"
#endif
#ifndef UYEL
#define UYEL "\e[4;33m"
#endif
#ifndef UBLU
#define UBLU "\e[4;34m"
#endif
#ifndef UMAG
#define UMAG "\e[4;35m"
#endif
#ifndef UCYN
#define UCYN "\e[4;36m"
#endif
#ifndef UWHT
#define UWHT "\e[4;37m"
#endif

/*Regular background*/
#ifndef BLKB
#define BLKB "\e[40m"
#endif
#ifndef REDB
#define REDB "\e[41m"
#endif
#ifndef GRNB
#define GRNB "\e[42m"
#endif
#ifndef YELB
#define YELB "\e[43m"
#endif
#ifndef BLUB
#define BLUB "\e[44m"
#endif
#ifndef MAGB
#define MAGB "\e[45m"
#endif
#ifndef CYNB
#define CYNB "\e[46m"
#endif
#ifndef WHTB
#define WHTB "\e[47m"
#endif

/*High intensty background */
#ifndef BLKHB
#define BLKHB "\e[0;100m"
#endif
#ifndef REDHB
#define REDHB "\e[0;101m"
#endif
#ifndef GRNHB
#define GRNHB "\e[0;102m"
#endif
#ifndef YELHB
#define YELHB "\e[0;103m"
#endif
#ifndef BLUHB
#define BLUHB "\e[0;104m"
#endif
#ifndef MAGHB
#define MAGHB "\e[0;105m"
#endif
#ifndef CYNHB
#define CYNHB "\e[0;106m"
#endif
#ifndef WHTHB
#define WHTHB "\e[0;107m"
#endif

/*High intensty text*/
#ifndef HBLK
#define HBLK "\e[0;90m"
#endif
#ifndef HRED
#define HRED "\e[0;91m"
#endif
#ifndef HGRN
#define HGRN "\e[0;92m"
#endif
#ifndef HYEL
#define HYEL "\e[0;93m"
#endif
#ifndef HBLU
#define HBLU "\e[0;94m"
#endif
#ifndef HMAG
#define HMAG "\e[0;95m"
#endif
#ifndef HCYN
#define HCYN "\e[0;96m"
#endif
#ifndef HWHT
#define HWHT "\e[0;97m"
#endif

/*Bold high intensity text*/
#ifndef BHBLK
#define BHBLK "\e[1;90m"
#endif
#ifndef BHRED
#define BHRED "\e[1;91m"
#endif
#ifndef BHGRN
#define BHGRN "\e[1;92m"
#endif
#ifndef BHYEL
#define BHYEL "\e[1;93m"
#endif
#ifndef BHBLU
#define BHBLU "\e[1;94m"
#endif
#ifndef BHMAG
#define BHMAG "\e[1;95m"
#endif
#ifndef BHCYN
#define BHCYN "\e[1;96m"
#endif
#ifndef BHWHT
#define BHWHT "\e[1;97m"
#endif

#ifndef CRESET
#define CRESET "\e[0m"
#endif
#ifndef COLOR_RESET
#define COLOR_RESET "\e[0m"
#endif

#else /* COLORS_ENABLED */

/*Regular text*/
#ifndef BLK
#define BLK ""
#endif
#ifndef RED
#define RED ""
#endif
#ifndef GRN
#define GRN ""
#endif
#ifndef YEL
#define YEL ""
#endif
#ifndef BLU
#define BLU ""
#endif
#ifndef MAG
#define MAG ""
#endif
#ifndef CYN
#define CYN ""
#endif
#ifndef WHT
#define WHT ""
#endif 

/*Regular bold text*/
#ifndef BBLK
#define BBLK ""
#endif
#ifndef BRED
#define BRED ""
#endif
#ifndef BGRN
#define BGRN ""
#endif
#ifndef BYEL
#define BYEL ""
#endif
#ifndef BBLU
#define BBLU ""
#endif
#ifndef BMAG
#define BMAG ""
#endif
#ifndef BCYN
#define BCYN ""
#endif
#ifndef BWHT
#define BWHT ""
#endif

/*Regular underline text*/
#ifndef UBLK
#define UBLK ""
#endif
#ifndef URED
#define URED ""
#endif
#ifndef UGRN
#define UGRN ""
#endif
#ifndef UYEL
#define UYEL ""
#endif
#ifndef UBLU
#define UBLU ""
#endif
#ifndef UMAG
#define UMAG ""
#endif
#ifndef UCYN
#define UCYN ""
#endif
#ifndef UWHT
#define UWHT ""
#endif

/*Regular background*/
#ifndef BLKB
#define BLKB ""
#endif
#ifndef REDB
#define REDB ""
#endif
#ifndef GRNB
#define GRNB ""
#endif
#ifndef YELB
#define YELB ""
#endif
#ifndef BLUB
#define BLUB ""
#endif
#ifndef MAGB
#define MAGB ""
#endif
#ifndef CYNB
#define CYNB ""
#endif
#ifndef WHTB
#define WHTB ""
#endif

/*High intensty background */
#ifndef BLKHB
#define BLKHB ""
#endif
#ifndef REDHB
#define REDHB ""
#endif
#ifndef GRNHB
#define GRNHB ""
#endif
#ifndef YELHB
#define YELHB ""
#endif
#ifndef BLUHB
#define BLUHB ""
#endif
#ifndef MAGHB
#define MAGHB ""
#endif
#ifndef CYNHB
#define CYNHB ""
#endif
#ifndef WHTHB
#define WHTHB ""
#endif

/*High intensty text*/
#ifndef HBLK
#define HBLK ""
#endif
#ifndef HRED
#define HRED ""
#endif
#ifndef HGRN
#define HGRN ""
#endif
#ifndef HYEL
#define HYEL ""
#endif
#ifndef HBLU
#define HBLU ""
#endif
#ifndef HMAG
#define HMAG ""
#endif
#ifndef HCYN
#define HCYN ""
#endif
#ifndef HWHT
#define HWHT ""
#endif

/*Bold high intensity text*/
#ifndef BHBLK
#define BHBLK ""
#endif
#ifndef BHRED
#define BHRED ""
#endif
#ifndef BHGRN
#define BHGRN ""
#endif
#ifndef BHYEL
#define BHYEL ""
#endif
#ifndef BHBLU
#define BHBLU ""
#endif
#ifndef BHMAG
#define BHMAG ""
#endif
#ifndef BHCYN
#define BHCYN ""
#endif
#ifndef BHWHT
#define BHWHT ""
#endif

#ifndef CRESET
#define CRESET ""
#endif
#ifndef COLOR_RESET
#define COLOR_RESET ""
#endif

#endif /* COLORS_ENABLED*/

#endif /* COLORS_H */
