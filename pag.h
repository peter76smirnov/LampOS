#ifndef	__PAG__H__
#define	__PAG__H__

#define	PG_PRESENT	(1 << 0)
#define	PG_RW		(1 << 1)
#define	PG_US		(1 << 2)
#define	PG_ACCESS	(1 << 5)
#define	PG_DIRTY	(1 << 6)

#define	PGTBLSIZ	8

typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

uint32_t pgdirectory[1024]	__attribute__ ((aligned(0x1000)));
uint32_t pgtable[8][1024]	__attribute__ ((aligned(0x1000)));

void pginit();

paddr_t pgalloc();

void pgfree(paddr_t addr);

vaddr_t mappage(vaddr_t t0, vaddr_t dst, paddr_t src);

void *getpage(paddr_t addr);

#endif

