enum Rarity { Common, Uncommon, Rare, Mythic };

typedef struct _card {
	unsigned int id;
	char *name;
	char *cost;
	unsigned int coverted_cost;
	char *type;
	char *text;
	char *stats;
	enum Rarity rarity;
} CARD;

