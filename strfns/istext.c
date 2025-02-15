static const char RCSID[]="@(#)$Id: istext.c,v 1.1 1998/10/07 16:19:56 rk Exp $";

/* istext - returns a status of 0 if the file(s) given on the command line
 *	are purely text files, else returns 1
 */

static char _ISTEXT_[256] = {
/* Oct   Dec   Hex   Char		istext */
/* --------------------------	*/
/* 000   0     00    NUL '\0'	*/	0,
/* 001   1     01    SOH	*/	0,
/* 002   2     02    STX	*/	0,
/* 003   3     03    ETX	*/	0,
/* 004   4     04    EOT	*/	0,
/* 005   5     05    ENQ	*/	0,
/* 006   6     06    ACK	*/	0,
/* 007   7     07    BEL '\a'	*/	1,
/* 010   8     08    BS  '\b'	*/	1,
/* 011   9     09    HT  '\t'	*/	1,
/* 012   10    0A    LF  '\n'	*/	1,
/* 013   11    0B    VT  '\v'	*/	1,
/* 014   12    0C    FF  '\f'	*/	1,
/* 015   13    0D    CR  '\r'	*/	1,
/* 016   14    0E    SO		*/	0,
/* 017   15    0F    SI		*/	0,
/* 020   16    10    DLE	*/	0,
/* 021   17    11    DC1	*/	0,
/* 022   18    12    DC2	*/	0,
/* 023   19    13    DC3	*/	0,
/* 024   20    14    DC4	*/	0,
/* 025   21    15    NAK	*/	0,
/* 026   22    16    SYN	*/	0,
/* 027   23    17    ETB	*/	0,
/* 030   24    18    CAN	*/	0,
/* 031   25    19    EM		*/	0,
/* 032   26    1A    SUB	*/	0,
/* 033   27    1B    ESC	*/	1,
/* 034   28    1C    FS		*/	0,
/* 035   29    1D    GS		*/	0,
/* 036   30    1E    RS		*/	0,
/* 037   31    1F    US		*/	0,
/* 040   32    20    SPACE	*/	1,
/* 041   33    21    !		*/	1,
/* 042   34    22    "		*/	1,
/* 043   35    23    #		*/	1,
/* 044   36    24    $		*/	1,
/* 045   37    25    %		*/	1,
/* 046   38    26    &		*/	1,
/* 047   39    27    '		*/	1,
/* 050   40    28    (		*/	1,
/* 051   41    29    )		*/	1,
/* 052   42    2A    *		*/	1,
/* 053   43    2B    +		*/	1,
/* 054   44    2C    ,		*/	1,
/* 055   45    2D    -		*/	1,
/* 056   46    2E    .		*/	1,
/* 057   47    2F    /		*/	1,
/* 060   48    30    0		*/	1,
/* 061   49    31    1		*/	1,
/* 062   50    32    2		*/	1,
/* 063   51    33    3		*/	1,
/* 064   52    34    4		*/	1,
/* 065   53    35    5		*/	1,
/* 066   54    36    6		*/	1,
/* 067   55    37    7		*/	1,
/* 070   56    38    8		*/	1,
/* 071   57    39    9		*/	1,
/* 072   58    3A    :		*/	1,
/* 073   59    3B    ;		*/	1,
/* 074   60    3C    <		*/	1,
/* 075   61    3D    =		*/	1,
/* 076   62    3E    >		*/	1,
/* 077   63    3F    ?		*/	1,
/* 100   64    40    @		*/	1,
/* 101   65    41    A		*/	1,
/* 102   66    42    B		*/	1,
/* 103   67    43    C		*/	1,
/* 104   68    44    D		*/	1,
/* 105   69    45    E		*/	1,
/* 106   70    46    F		*/	1,
/* 107   71    47    G		*/	1,
/* 110   72    48    H		*/	1,
/* 111   73    49    I		*/	1,
/* 112   74    4A    J		*/	1,
/* 113   75    4B    K		*/	1,
/* 114   76    4C    L		*/	1,
/* 115   77    4D    M		*/	1,
/* 116   78    4E    N		*/	1,
/* 117   79    4F    O		*/	1,
/* 120   80    50    P		*/	1,
/* 121   81    51    Q		*/	1,
/* 122   82    52    R		*/	1,
/* 123   83    53    S		*/	1,
/* 124   84    54    T		*/	1,
/* 125   85    55    U		*/	1,
/* 126   86    56    V		*/	1,
/* 127   87    57    W		*/	1,
/* 130   88    58    X		*/	1,
/* 131   89    59    Y		*/	1,
/* 132   90    5A    Z		*/	1,
/* 133   91    5B    [		*/	1,
/* 134   92    5C    \ '\\'	*/	1,
/* 135   93    5D    ]		*/	1,
/* 136   94    5E    ^		*/	1,
/* 137   95    5F    _		*/	1,
/* 140   96    60    `		*/	1,
/* 141   97    61    a		*/	1,
/* 142   98    62    b		*/	1,
/* 143   99    63    c		*/	1,
/* 144   100   64    d		*/	1,
/* 145   101   65    e		*/	1,
/* 146   102   66    f		*/	1,
/* 147   103   67    g		*/	1,
/* 150   104   68    h		*/	1,
/* 151   105   69    i		*/	1,
/* 152   106   6A    j		*/	1,
/* 153   107   6B    k		*/	1,
/* 154   108   6C    l		*/	1,
/* 155   109   6D    m		*/	1,
/* 156   110   6E    n		*/	1,
/* 157   111   6F    o		*/	1,
/* 160   112   70    p		*/	1,
/* 161   113   71    q		*/	1,
/* 162   114   72    r		*/	1,
/* 163   115   73    s		*/	1,
/* 164   116   74    t		*/	1,
/* 165   117   75    u		*/	1,
/* 166   118   76    v		*/	1,
/* 167   119   77    w		*/	1,
/* 170   120   78    x		*/	1,
/* 171   121   79    y		*/	1,
/* 172   122   7A    z		*/	1,
/* 173   123   7B    {		*/	1,
/* 174   124   7C    |		*/	1,
/* 175   125   7D    }		*/	1,
/* 176   126   7E    ~		*/	1,
/* 177   127   7F    DEL	*/	0,
/* 128-255 */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0};

/* returns non-zero if c is a text character else 0 */
int istext(int c) {
	c &= 255;			/* sure in range */
	return (int) _ISTEXT_[c];
}
