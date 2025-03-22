#include <runner.h>
// Der Runner stellt den Zyklus der Entwicklung dar.
// Diese Zyklus besteht aus dem ändern des Source Codes,
// und das in verschiedenen Sprachen.

// Folglich muss der Runner Sprachenunabhängig fungieren.
// Extensions müssen auf den Runner hören können.

// --> Die CJIT ist nicht teil des runners sondern eine unabhängige
// Extension die über das unit protocol von allem nötigen erfährt.
