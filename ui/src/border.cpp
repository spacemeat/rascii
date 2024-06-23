#include "ui.hpp"

namspeace rascii
{

auto box_chars = [
    //      MSB <----> LSB
    //      DRUL           DRUL           DRUL           DRUL           DRUL           DRUL           DRUL           DRUL
	u8" ",/*NNNN*/ u8"╴",/*NNNS*/ u8"╸",/*NNNB*/ u8" ",/*NNND*/ u8"╵",/*NNSN*/ u8"┘",/*NNSS*/ u8"┙",/*NNSB*/ u8"╛",/*NNSD*/
    u8"╹",/*NNBN*/ u8"┚",/*NNBS*/ u8"┛",/*NNBB*/ u8" ",/*NNBD*/ u8" ",/*NNDN*/ u8"╜",/*NNDS*/ u8" ",/*NNDB*/ u8"╝",/*NNDD*/
    u8"╶",/*NSNN*/ u8"─",/*NSNS*/ u8"╾",/*NSNB*/ u8" ",/*NSND*/ u8"└",/*NSSN*/ u8"┴",/*NSSS*/ u8"┵",/*NSSB*/ u8" ",/*NSSD*/
    u8"┖",/*NSBN*/ u8"┸",/*NSBS*/ u8"┹",/*NSBB*/ u8" ",/*NSBD*/ u8"╙",/*NSDN*/ u8"╨",/*NSDS*/ u8" ",/*NSDB*/ u8" ",/*NSDD*/
	u8"╺",/*NBNN*/ u8"╼",/*NBNS*/ u8"━",/*NBNB*/ u8" ",/*NBND*/ u8"┕",/*NBSN*/ u8"┶",/*NBSS*/ u8"┷",/*NBSB*/ u8" ",/*NBSD*/
    u8"┗",/*NBBN*/ u8"┺",/*NBBS*/ u8"┻",/*NBBB*/ u8" ",/*NBBD*/ u8" ",/*NBDN*/ u8" ",/*NBDS*/ u8" ",/*NBDB*/ u8" ",/*NBDD*/
    u8" ",/*NDNN*/ u8" ",/*NDNS*/ u8" ",/*NDNB*/ u8"═",/*NDND*/ u8"╘",/*NDSN*/ u8" ",/*NDSS*/ u8" ",/*NDSB*/ u8"╧",/*NDSD*/
    u8" ",/*NDBN*/ u8" ",/*NDBS*/ u8" ",/*NDBB*/ u8" ",/*NDBD*/ u8"╚",/*NDDN*/ u8" ",/*NDDS*/ u8" ",/*NDDB*/ u8"╩",/*NDDD*/
	u8"╷",/*SNNN*/ u8"┐",/*SNNS*/ u8"┑",/*SNNB*/ u8"╕",/*SNND*/ u8"│",/*SNSN*/ u8"┤",/*SNSS*/ u8"┥",/*SNSB*/ u8"╡",/*SNSD*/
    u8"╿",/*SNBN*/ u8"┦",/*SNBS*/ u8"┩",/*SNBB*/ u8" ",/*SNBD*/ u8" ",/*SNDN*/ u8" ",/*SNDS*/ u8" ",/*SNDB*/ u8" ",/*SNDD*/
    u8"┌",/*SSNN*/ u8"┬",/*SSNS*/ u8"┭",/*SSNB*/ u8" ",/*SSND*/ u8"├",/*SSSN*/ u8"┼",/*SSSS*/ u8"┽",/*SSSB*/ u8" ",/*SSSD*/
    u8"┞",/*SSBN*/ u8"╀",/*SSBS*/ u8"╃",/*SSBB*/ u8" ",/*SSBD*/ u8" ",/*SSDN*/ u8" ",/*SSDS*/ u8" ",/*SSDB*/ u8" ",/*SSDD*/
	u8"┍",/*SBNN*/ u8"┮",/*SBNS*/ u8"┯",/*SBNB*/ u8" ",/*SBND*/ u8"┝",/*SBSN*/ u8"┾",/*SBSS*/ u8"┿",/*SBSB*/ u8" ",/*SBSD*/
    u8"┡",/*SBBN*/ u8"╄",/*SBBS*/ u8"╇",/*SBBB*/ u8" ",/*SBBD*/ u8" ",/*SBDN*/ u8" ",/*SBDS*/ u8" ",/*SBDB*/ u8" ",/*SBDD*/
    u8"╒",/*SDNN*/ u8" ",/*SDNS*/ u8" ",/*SDNB*/ u8"╤",/*SDND*/ u8"╞",/*SDSN*/ u8" ",/*SDSS*/ u8" ",/*SDSB*/ u8"╪",/*SDSD*/
    u8" ",/*SDBN*/ u8" ",/*SDBS*/ u8" ",/*SDBB*/ u8" ",/*SDBD*/ u8" ",/*SDDN*/ u8" ",/*SDDS*/ u8" ",/*SDDB*/ u8" ",/*SDDD*/
	u8"╻",/*BNNN*/ u8"┒",/*BNNS*/ u8"┓",/*BNNB*/ u8" ",/*BNND*/ u8"╽",/*BNSN*/ u8"┧",/*BNSS*/ u8"┪",/*BNSB*/ u8" ",/*BNSD*/
    u8"┃",/*BNBN*/ u8"┨",/*BNBS*/ u8"┫",/*BNBB*/ u8" ",/*BNBD*/ u8" ",/*BNDN*/ u8" ",/*BNDS*/ u8" ",/*BNDB*/ u8" ",/*BNDD*/
    u8"┎",/*BSNN*/ u8"┰",/*BSNS*/ u8"┱",/*BSNB*/ u8" ",/*BSND*/ u8"┟",/*BSSN*/ u8"╁",/*BSSS*/ u8"╅",/*BSSB*/ u8" ",/*BSSD*/
    u8"┠",/*BSBN*/ u8"╂",/*BSBS*/ u8"╉",/*BSBB*/ u8" ",/*BSBD*/ u8" ",/*BSDN*/ u8" ",/*BSDS*/ u8" ",/*BSDB*/ u8" ",/*BSDD*/
	u8"┏",/*BBNN*/ u8"┲",/*BBNS*/ u8"┳",/*BBNB*/ u8" ",/*BBND*/ u8"┢",/*BBSN*/ u8"╆",/*BBSS*/ u8"╈",/*BBSB*/ u8" ",/*BBSD*/
    u8"┣",/*BBBN*/ u8"╊",/*BBBS*/ u8"╋",/*BBBB*/ u8" ",/*BBBD*/ u8" ",/*BBDN*/ u8" ",/*BBDS*/ u8" ",/*BBDB*/ u8" ",/*BBDD*/
    u8" ",/*BDNN*/ u8" ",/*BDNS*/ u8" ",/*BDNB*/ u8" ",/*BDND*/ u8" ",/*BDSN*/ u8" ",/*BDSS*/ u8" ",/*BDSB*/ u8" ",/*BDSD*/
    u8" ",/*BDBN*/ u8" ",/*BDBS*/ u8" ",/*BDBB*/ u8" ",/*BDBD*/ u8" ",/*BDDN*/ u8" ",/*BDDS*/ u8" ",/*BDDB*/ u8" ",/*BDDD*/
	u8" ",/*DNNN*/ u8"╖",/*DNNS*/ u8" ",/*DNNB*/ u8"╗",/*DNND*/ u8" ",/*DNSN*/ u8" ",/*DNSS*/ u8" ",/*DNSB*/ u8" ",/*DNSD*/
    u8" ",/*DNBN*/ u8" ",/*DNBS*/ u8" ",/*DNBB*/ u8" ",/*DNBD*/ u8"║",/*DNDN*/ u8"╢",/*DNDS*/ u8" ",/*DNDB*/ u8"╣",/*DNDD*/
    u8"╓",/*DSNN*/ u8"╥",/*DSNS*/ u8" ",/*DSNB*/ u8" ",/*DSND*/ u8" ",/*DSSN*/ u8" ",/*DSSS*/ u8" ",/*DSSB*/ u8" ",/*DSSD*/
    u8" ",/*DSBN*/ u8" ",/*DSBS*/ u8" ",/*DSBB*/ u8" ",/*DSBD*/ u8"╟",/*DSDN*/ u8"╫",/*DSDS*/ u8" ",/*DSDB*/ u8" ",/*DSDD*/
	u8" ",/*DBNN*/ u8" ",/*DBNS*/ u8" ",/*DBNB*/ u8" ",/*DBND*/ u8" ",/*DBSN*/ u8" ",/*DBSS*/ u8" ",/*DBSB*/ u8" ",/*DBSD*/
    u8" ",/*DBBN*/ u8" ",/*DBBS*/ u8" ",/*DBBB*/ u8" ",/*DBBD*/ u8" ",/*DBDN*/ u8" ",/*DBDS*/ u8" ",/*DBDB*/ u8" ",/*DBDD*/
    u8"╔",/*DDNN*/ u8" ",/*DDNS*/ u8" ",/*DDNB*/ u8"╦",/*DDND*/ u8" ",/*DDSN*/ u8" ",/*DDSS*/ u8" ",/*DDSB*/ u8" ",/*DDSD*/
    u8" ",/*DDBN*/ u8" ",/*DDBS*/ u8" ",/*DDBB*/ u8" ",/*DDBD*/ u8"╠",/*DDDN*/ u8" ",/*DDDS*/ u8" ",/*DDDB*/ u8"╬",/*DDDD*/

/*
constexpr enum class BoxConnection { LEFT, UP, RIGHT, DOWN };

constexpr enum class BoxCellType {
	NONE, SINGLE, THICK, DOUBLE,
	ROUND,															// only corners
	SEG_2, SEG_2_THICK, SEG_3, SEG_3_THICK, SEG_4, SEG_4_THICK,		// only LR and UD
*/
static std::u8string_view const get_box_chars(std::array<BoxCellType, 4> const & connections)
{
	uint code = 0;
	for (int i = 0; i < 4; ++i)
	{
		code += (static_cast<uint>(connections[i]) << i * 2);
	}
	
	return box_chars[code];
}

std::u8string_view const get_box_cell(std::array<BoxCellType, 4> const & connections)
{
	uint line_types_represented = 0;
	for (int i = 0; i < 4; ++i)
	{
		line_types_represented |= (1 << connections[i]);
	}

	if (line_types_represented & static_cast<uint>(BoxCellType::ROUND))
	{
		// modify all that aren't strictly round corners to single-line, and get the glyph
		if (connections == std::array { BoxCellType::ROUND, BoxCellType::ROUND, BoxCellType::NONE, BoxCellType::NONE })
			{ return u8"╯"; }
	    if (connections == std::array { BoxCellType::NONE, BoxCellType::ROUND, BoxCellType::ROUND, BoxCellType::NONE })
			{ return u8"╰"; }
	    if (connections == std::array { BoxCellType::NONE, BoxCellType::NONE, BoxCellType::ROUND, BoxCellType::ROUND })
			{ return u8"╭"; }
		if (connections == std::array { BoxCellType::ROUND, BoxCellType::NONE, BoxCellType::NONE, BoxCellType::ROUND })
			{ return u8"╮"; }
		std::array<BoxCellType, 4> recon = connections;
		for (auto & bct : recon)
		{
			if (bct == BoxCellType::ROUND)
				{ bct = BoxCellType::SINGLE; }
		}
		return get_box_chars(recond);
	}
	
}
