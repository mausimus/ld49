#include "Assets.h"

Assets::Assets()
{
    GenerateChars();
}

void Assets::Load() { }

void Assets::Unload() { }

void Assets::Render(float thick)
{
    for(const auto& c : _chars)
    {
        c.second->Render(thick);
    }
    for(const auto& c : _chars2)
    {
        c.second->Render(thick);
    }
    for(const auto& c : _chars4)
    {
        c.second->Render(thick);
    }
}

void Assets::AddChar(char c, VectorObject* vo)
{
    vo->Flip(0, 1);
    vo->Center();
    vo->CenterY(-CHAR_MIDPOINT);
    vo->Scale(CHAR_SCALE);
    _chars.insert(std::pair<char, VectorObject*>(c, vo));

    auto vo2 = new VectorObject(*vo);
    vo2->Scale(CHAR_SCALE * 2.0f);
    vo2->_width = vo->_width * 2;
    _chars2.insert(std::pair<char, VectorObject*>(c, vo2));

    auto vo4 = new VectorObject(*vo);
    vo4->Scale(CHAR_SCALE * 4.0f);
    vo4->_width = vo->_width * 4;
    _chars4.insert(std::pair<char, VectorObject*>(c, vo4));
}

VectorObject* Assets::GetChar(char c, int zoom)
{
    std::map<char, VectorObject*>& chars = zoom == 1 ? _chars : (zoom == 2 ? _chars2 : _chars4);
    const auto&                    fc    = chars.find(c);
    if(fc == chars.end())
    {
        return chars.at(' ');
    }
    return fc->second;
}

void Assets::GenerateChars()
{
    VectorObject* vo;

    vo         = new VectorObject();
    vo->_width = 0.1f;
    AddChar(' ', vo);

    vo = new VectorObject();
    vo->moveTo(0, 0);
    vo->lineTo(0, 15);
    vo->lineTo(3, 18);
    vo->lineTo(9, 18);
    vo->lineTo(12, 15);
    vo->lineTo(12, 0);
    vo->moveTo(0, 9);
    vo->lineTo(12, 9);
    AddChar('A', vo);

    vo = new VectorObject();
    vo->moveTo(18, 0);
    vo->lineTo(18, 18);
    vo->lineTo(27, 18);
    vo->lineTo(30, 15);
    vo->lineTo(30, 12);
    vo->lineTo(27, 9);
    vo->lineTo(18, 9);
    vo->moveTo(27, 9);
    vo->lineTo(30, 6);
    vo->lineTo(30, 3);
    vo->lineTo(27, 0);
    vo->lineTo(18, 0);
    AddChar('B', vo);

    vo = new VectorObject();
    vo->moveTo(48, 3);
    vo->lineTo(45, 0);
    vo->lineTo(39, 0);
    vo->lineTo(36, 3);
    vo->lineTo(36, 15);
    vo->lineTo(39, 18);
    vo->lineTo(45, 18);
    vo->lineTo(48, 15);
    AddChar('C', vo);

    vo = new VectorObject();
    vo->moveTo(54, 0);
    vo->lineTo(54, 18);
    vo->lineTo(63, 18);
    vo->lineTo(66, 15);
    vo->lineTo(66, 3);
    vo->lineTo(63, 0);
    vo->lineTo(54, 0);
    AddChar('D', vo);

    vo = new VectorObject();
    vo->moveTo(72, 0);
    vo->lineTo(72, 18);
    vo->lineTo(84, 18);
    vo->moveTo(72, 9);
    vo->lineTo(81, 9);
    vo->moveTo(72, 0);
    vo->lineTo(84, 0);
    AddChar('E', vo);

    vo = new VectorObject();
    vo->moveTo(90, 0);
    vo->lineTo(90, 18);
    vo->lineTo(102, 18);
    vo->moveTo(90, 9);
    vo->lineTo(99, 9);
    AddChar('F', vo);

    vo = new VectorObject();
    vo->moveTo(120, 15);
    vo->lineTo(117, 18);
    vo->lineTo(111, 18);
    vo->lineTo(108, 15);
    vo->lineTo(108, 3);
    vo->lineTo(111, 0);
    vo->lineTo(117, 0);
    vo->lineTo(120, 3);
    vo->lineTo(120, 8);
    vo->lineTo(113, 8);
    AddChar('G', vo);

    vo = new VectorObject();
    vo->moveTo(126, 0);
    vo->lineTo(126, 18);
    vo->moveTo(138, 0);
    vo->lineTo(138, 18);
    vo->moveTo(126, 9);
    vo->lineTo(138, 9);
    AddChar('H', vo);

    vo = new VectorObject();
    vo->moveTo(146, 0);
    vo->lineTo(154, 0);
    vo->moveTo(150, 0);
    vo->lineTo(150, 18);
    vo->moveTo(146, 18);
    vo->lineTo(154, 18);
    AddChar('I', vo);

    vo = new VectorObject();
    vo->moveTo(162, 2);
    vo->lineTo(165, 0);
    vo->lineTo(167, 0);
    vo->lineTo(170, 2);
    vo->lineTo(170, 18);
    vo->moveTo(166, 18);
    vo->lineTo(174, 18);
    AddChar('J', vo);

    vo = new VectorObject();
    vo->moveTo(180, 0);
    vo->lineTo(180, 18);
    vo->moveTo(192, 18);
    vo->lineTo(180, 6);
    vo->moveTo(183, 9);
    vo->lineTo(192, 0);
    AddChar('K', vo);

    vo = new VectorObject();
    vo->moveTo(198, 0);
    vo->lineTo(198, 18);
    vo->moveTo(198, 0);
    vo->lineTo(210, 0);
    AddChar('L', vo);

    vo = new VectorObject();
    vo->moveTo(216, 0);
    vo->lineTo(216, 18);
    vo->lineTo(222, 5);
    vo->lineTo(228, 18);
    vo->lineTo(228, 0);
    AddChar('M', vo);

    vo = new VectorObject();
    vo->moveTo(234, 0);
    vo->lineTo(234, 18);
    vo->lineTo(246, 0);
    vo->lineTo(246, 18);
    AddChar('N', vo);

    vo = new VectorObject();
    vo->moveTo(255, 0);
    vo->lineTo(252, 3);
    vo->lineTo(252, 15);
    vo->lineTo(255, 18);
    vo->lineTo(261, 18);
    vo->lineTo(264, 15);
    vo->lineTo(264, 3);
    vo->lineTo(261, 0);
    vo->lineTo(255, 0);
    AddChar('O', vo);

    vo = new VectorObject();
    vo->moveTo(270, 0);
    vo->lineTo(270, 18);
    vo->lineTo(279, 18);
    vo->lineTo(282, 15);
    vo->lineTo(282, 11);
    vo->lineTo(279, 8);
    vo->lineTo(270, 8);
    AddChar('P', vo);

    vo = new VectorObject();
    vo->moveTo(291, 0);
    vo->lineTo(288, 3);
    vo->lineTo(288, 15);
    vo->lineTo(291, 18);
    vo->lineTo(297, 18);
    vo->lineTo(300, 15);
    vo->lineTo(300, 3);
    vo->lineTo(297, 0);
    vo->lineTo(291, 0);
    vo->moveTo(295, 5);
    vo->lineTo(302, -2);
    AddChar('Q', vo);

    vo = new VectorObject();
    vo->moveTo(306, 0);
    vo->lineTo(306, 18);
    vo->lineTo(315, 18);
    vo->lineTo(318, 15);
    vo->lineTo(318, 11);
    vo->lineTo(315, 8);
    vo->lineTo(306, 8);
    vo->moveTo(313, 8);
    vo->lineTo(318, 0);
    AddChar('R', vo);

    vo = new VectorObject();
    vo->moveTo(324, 2);
    vo->lineTo(327, 0);
    vo->lineTo(333, 0);
    vo->lineTo(336, 3);
    vo->lineTo(336, 6);
    vo->lineTo(333, 9);
    vo->lineTo(327, 9);
    vo->lineTo(324, 12);
    vo->lineTo(324, 15);
    vo->lineTo(327, 18);
    vo->lineTo(333, 18);
    vo->lineTo(336, 16);
    AddChar('S', vo);

    vo = new VectorObject();
    vo->moveTo(348, 0);
    vo->lineTo(348, 18);
    vo->moveTo(342, 18);
    vo->lineTo(354, 18);
    AddChar('T', vo);

    vo = new VectorObject();
    vo->moveTo(360, 18);
    vo->lineTo(360, 3);
    vo->lineTo(363, 0);
    vo->lineTo(369, 0);
    vo->lineTo(372, 3);
    vo->lineTo(372, 18);
    AddChar('U', vo);

    vo = new VectorObject();
    vo->moveTo(0, 18);
    vo->lineTo(2, 0);
    vo->lineTo(6, 12);
    vo->lineTo(10, 0);
    vo->lineTo(12, 18);
    AddChar('W', vo);

    vo = new VectorObject();
    vo->moveTo(378, 18);
    vo->lineTo(384, 0);
    vo->lineTo(390, 18);
    AddChar('V', vo);

    vo = new VectorObject();
    vo->moveTo(396, 0);
    vo->lineTo(408, 18);
    vo->moveTo(396, 18);
    vo->lineTo(408, 0);
    AddChar('X', vo);

    vo = new VectorObject();
    vo->moveTo(420, 0);
    vo->lineTo(420, 7);
    vo->lineTo(414, 18);
    vo->moveTo(420, 7);
    vo->lineTo(426, 18);
    AddChar('Y', vo);

    vo = new VectorObject();
    vo->moveTo(432, 0);
    vo->lineTo(444, 18);
    vo->lineTo(432, 18);
    vo->moveTo(444, 0);
    vo->lineTo(432, 0);
    AddChar('Z', vo);

    vo = new VectorObject();
    vo->moveTo(452, 3);
    vo->lineTo(460, 15);
    vo->moveTo(462, 12);
    vo->lineTo(462, 6);
    vo->lineTo(459, 0);
    vo->lineTo(453, 0);
    vo->lineTo(450, 6);
    vo->lineTo(450, 12);
    vo->lineTo(453, 18);
    vo->lineTo(459, 18);
    vo->lineTo(462, 12);
    AddChar('0', vo);

    vo = new VectorObject();
    vo->moveTo(474, 0);
    vo->lineTo(474, 18);
    vo->lineTo(468, 12);
    AddChar('1', vo);

    vo = new VectorObject();
    vo->moveTo(486, 15);
    vo->lineTo(489, 18);
    vo->lineTo(495, 18);
    vo->lineTo(498, 15);
    vo->lineTo(498, 11);
    vo->lineTo(488, 5);
    vo->lineTo(486, 0);
    vo->lineTo(498, 0);
    AddChar('2', vo);

    vo = new VectorObject();
    vo->moveTo(504, 16);
    vo->lineTo(507, 18);
    vo->lineTo(513, 18);
    vo->lineTo(516, 15);
    vo->lineTo(516, 11);
    vo->lineTo(513, 9);
    vo->lineTo(507, 9);
    vo->moveTo(513, 9);
    vo->lineTo(516, 7);
    vo->lineTo(516, 3);
    vo->lineTo(513, 0);
    vo->lineTo(507, 0);
    vo->lineTo(504, 2);
    AddChar('3', vo);

    vo = new VectorObject();
    vo->moveTo(531, 0);
    vo->lineTo(531, 18);
    vo->lineTo(522, 6);
    vo->lineTo(534, 6);
    AddChar('4', vo);

    vo = new VectorObject();
    vo->moveTo(540, 2);
    vo->lineTo(543, 0);
    vo->lineTo(549, 0);
    vo->lineTo(552, 2);
    vo->lineTo(552, 8);
    vo->lineTo(549, 10);
    vo->lineTo(543, 10);
    vo->lineTo(540, 9);
    vo->lineTo(542, 18);
    vo->lineTo(552, 18);
    AddChar('5', vo);

    vo = new VectorObject();
    vo->moveTo(558, 7);
    vo->lineTo(561, 10);
    vo->lineTo(567, 10);
    vo->lineTo(570, 7);
    vo->lineTo(570, 3);
    vo->lineTo(567, 0);
    vo->lineTo(561, 0);
    vo->lineTo(558, 3);
    vo->lineTo(558, 10);
    vo->lineTo(561, 15);
    vo->lineTo(565, 18);
    vo->lineTo(568, 18);
    AddChar('6', vo);

    vo = new VectorObject();
    vo->moveTo(576, 18);
    vo->lineTo(588, 18);
    vo->lineTo(580, 0);
    AddChar('7', vo);

    vo = new VectorObject();
    vo->moveTo(597, 10);
    vo->lineTo(594, 13);
    vo->lineTo(594, 16);
    vo->lineTo(597, 19);
    vo->lineTo(603, 19);
    vo->lineTo(606, 16);
    vo->lineTo(606, 13);
    vo->lineTo(603, 10);
    vo->lineTo(597, 10);
    vo->lineTo(594, 7);
    vo->lineTo(594, 3);
    vo->lineTo(597, 0);
    vo->lineTo(603, 0);
    vo->lineTo(606, 3);
    vo->lineTo(606, 7);
    vo->lineTo(603, 10);
    AddChar('8', vo);

    vo = new VectorObject();
    vo->moveTo(614, 0);
    vo->lineTo(617, 0);
    vo->lineTo(621, 3);
    vo->lineTo(624, 8);
    vo->lineTo(624, 15);
    vo->lineTo(621, 18);
    vo->lineTo(615, 18);
    vo->lineTo(612, 15);
    vo->lineTo(612, 11);
    vo->lineTo(615, 8);
    vo->lineTo(621, 8);
    vo->lineTo(624, 11);
    AddChar('9', vo);

    vo = new VectorObject();
    vo->moveTo(636, 1);
    vo->lineTo(636, -1);
    AddChar('.', vo);

    vo = new VectorObject();
    vo->moveTo(652, -4);
    vo->lineTo(654, 1);
    vo->lineTo(654, 1);
    AddChar(',', vo);

    vo = new VectorObject();
    vo->moveTo(671, -4);
    vo->lineTo(673, 0);
    vo->lineTo(673, 0);
    vo->moveTo(673, 10);
    vo->lineTo(673, 7);
    AddChar(';', vo);

    vo = new VectorObject();
    vo->moveTo(690, 2);
    vo->lineTo(690, 5);
    vo->moveTo(690, 11);
    vo->lineTo(690, 14);
    AddChar(':', vo);

    vo = new VectorObject();
    vo->moveTo(708, 2);
    vo->lineTo(708, 0);
    vo->moveTo(708, 5);
    vo->lineTo(708, 18);
    AddChar('!', vo);

    vo = new VectorObject();
    vo->moveTo(732, 2);
    vo->lineTo(730, 0);
    vo->lineTo(723, 0);
    vo->lineTo(720, 3);
    vo->lineTo(720, 15);
    vo->lineTo(723, 18);
    vo->lineTo(729, 18);
    vo->lineTo(732, 15);
    vo->lineTo(732, 6);
    vo->lineTo(725, 6);
    vo->lineTo(725, 13);
    vo->lineTo(732, 13);
    AddChar('@', vo);

    vo = new VectorObject();
    vo->moveTo(740, 0);
    vo->lineTo(742, 18);
    vo->moveTo(746, 0);
    vo->lineTo(748, 18);
    vo->moveTo(738, 13);
    vo->lineTo(750, 13);
    vo->moveTo(738, 5);
    vo->lineTo(750, 5);
    AddChar('#', vo);

    vo = new VectorObject();
    vo->moveTo(756, 3);
    vo->lineTo(759, 1);
    vo->lineTo(765, 1);
    vo->lineTo(768, 3);
    vo->lineTo(768, 7);
    vo->lineTo(765, 9);
    vo->lineTo(759, 9);
    vo->lineTo(756, 11);
    vo->lineTo(756, 15);
    vo->lineTo(759, 17);
    vo->lineTo(765, 17);
    vo->lineTo(768, 15);
    vo->moveTo(762, 19);
    vo->lineTo(762, -1);
    AddChar('$', vo);

    vo = new VectorObject();
    vo->moveTo(774, 0);
    vo->lineTo(786, 18);
    vo->moveTo(780, 14);
    vo->lineTo(777, 10);
    vo->lineTo(774, 14);
    vo->lineTo(777, 18);
    vo->lineTo(780, 14);
    vo->moveTo(783, 8);
    vo->lineTo(786, 4);
    vo->lineTo(783, 0);
    vo->lineTo(780, 4);
    vo->lineTo(783, 8);
    AddChar('%', vo);

    vo = new VectorObject();
    vo->moveTo(792, 7);
    vo->lineTo(798, 16);
    vo->lineTo(804, 7);
    AddChar('^', vo);

    vo = new VectorObject();
    vo->moveTo(822, 5);
    vo->lineTo(818, 0);
    vo->lineTo(812, 0);
    vo->lineTo(810, 4);
    vo->lineTo(819, 14);
    vo->lineTo(817, 18);
    vo->lineTo(813, 18);
    vo->lineTo(811, 14);
    vo->lineTo(822, 0);
    AddChar('&', vo);

    vo = new VectorObject();
    vo->moveTo(831, 2);
    vo->lineTo(837, 16);
    vo->moveTo(831, 16);
    vo->lineTo(837, 2);
    vo->moveTo(828, 9);
    vo->lineTo(840, 9);
    AddChar('*', vo);

    vo = new VectorObject();
    vo->moveTo(858, -2);
    vo->lineTo(852, 4);
    vo->lineTo(852, 14);
    vo->lineTo(858, 20);
    AddChar('(', vo);

    vo = new VectorObject();
    vo->moveTo(864, -2);
    vo->lineTo(870, 4);
    vo->lineTo(870, 14);
    vo->lineTo(864, 20);
    AddChar(')', vo);

    vo = new VectorObject();
    vo->moveTo(882, 9);
    vo->lineTo(894, 9);
    AddChar('-', vo);

    vo = new VectorObject();
    vo->moveTo(900, 4);
    vo->lineTo(912, 4);
    vo->moveTo(900, 14);
    vo->lineTo(912, 14);
    AddChar('=', vo);

    vo = new VectorObject();
    vo->moveTo(924, 2);
    vo->lineTo(924, 16);
    vo->moveTo(918, 9);
    vo->lineTo(930, 9);
    AddChar('+', vo);

    vo = new VectorObject();
    vo->moveTo(918, 0);
    vo->lineTo(936, 0);
    AddChar('_', vo);

    vo = new VectorObject();
    vo->moveTo(936, 0);
    vo->lineTo(948, 18);
    AddChar('/', vo);

    vo = new VectorObject();
    vo->moveTo(948, 0);
    vo->lineTo(936, 18);
    AddChar('\\', vo);

    vo = new VectorObject();
    vo->moveTo(954, 15);
    vo->lineTo(957, 18);
    vo->lineTo(963, 18);
    vo->lineTo(966, 15);
    vo->lineTo(966, 11);
    vo->lineTo(960, 7);
    vo->lineTo(960, 4);
    vo->moveTo(960, 2);
    vo->lineTo(960, 0);
    AddChar('?', vo);

    vo = new VectorObject();
    vo->moveTo(984, 0);
    vo->lineTo(972, 9);
    vo->lineTo(984, 18);
    AddChar('<', vo);

    vo = new VectorObject();
    vo->moveTo(990, 0);
    vo->lineTo(1002, 9);
    vo->lineTo(990, 18);
    AddChar('>', vo);

    vo = new VectorObject();
    vo->moveTo(1020, 20);
    vo->lineTo(1014, 20);
    vo->lineTo(1014, -2);
    vo->lineTo(1020, -2);
    AddChar('[', vo);

    vo = new VectorObject();
    vo->moveTo(1026, -2);
    vo->lineTo(1032, -2);
    vo->lineTo(1032, 20);
    vo->lineTo(1026, 20);
    AddChar(']', vo);

    vo = new VectorObject();
    vo->moveTo(1056, -2);
    vo->lineTo(1051, 1);
    vo->lineTo(1051, 6);
    vo->lineTo(1048, 9);
    vo->lineTo(1051, 12);
    vo->lineTo(1051, 17);
    vo->lineTo(1056, 20);
    AddChar('{', vo);

    vo = new VectorObject();
    vo->moveTo(1062, -2);
    vo->lineTo(1067, 1);
    vo->lineTo(1067, 6);
    vo->lineTo(1070, 9);
    vo->lineTo(1067, 12);
    vo->lineTo(1067, 17);
    vo->lineTo(1062, 20);
    AddChar('}', vo);

    vo = new VectorObject();
    //
    //    vo->moveTo(1080, 18);
    //vo->lineTo(1092, 0);

    vo->moveTo(1080, 18);
    vo->lineTo(1080, 16);
    AddChar('\'', vo);

    vo = new VectorObject();
    vo->moveTo(1103, 18);
    vo->lineTo(1103, 18);
    vo->lineTo(1105, 14);
    AddChar('`', vo);

    vo = new VectorObject();
    vo->moveTo(0, 9);
    vo->lineTo(6, 18);
    vo->lineTo(12, 9);
    vo->moveTo(6, 0);
    vo->lineTo(6, 18);
    AddChar('u', vo);

    vo = new VectorObject();
    vo->moveTo(0, 9);
    vo->lineTo(6, 0);
    vo->lineTo(12, 9);
    vo->moveTo(6, 18);
    vo->lineTo(6, 0);
    AddChar('d', vo);

    vo = new VectorObject();
    vo->moveTo(9, 3);
    vo->lineTo(0, 9);
    vo->lineTo(9, 15);
    vo->moveTo(18, 9);
    vo->lineTo(0, 9);
    AddChar('l', vo);

    vo = new VectorObject();
    vo->moveTo(9, 3);
    vo->lineTo(18, 9);
    vo->lineTo(9, 15);
    vo->moveTo(0, 9);
    vo->lineTo(18, 9);
    AddChar('r', vo);
}

VectorObject* Assets::Plane()
{
    auto  vo  = new VectorObject();
    auto& ctx = *vo;

    ctx.moveTo(39.286594, 35.986019);
    ctx.lineTo(39.498511, 25.929167);
    ctx.lineTo(42.144346, 22.716370);
    ctx.lineTo(44.601191, 25.740179);
    ctx.lineTo(44.852248, 35.925918);
    ctx.lineTo(57.452381, 45.829613);
    ctx.lineTo(58.208334, 49.892857);
    ctx.lineTo(44.723361, 44.905634);
    ctx.lineTo(44.879929, 51.172604);
    ctx.lineTo(48.475452, 56.337351);
    ctx.lineTo(41.958720, 55.011077);
    ctx.lineTo(34.773814, 56.431845);
    ctx.lineTo(38.931553, 51.140179);
    ctx.lineTo(39.120540, 44.884673);
    ctx.lineTo(25.314913, 49.996033);
    ctx.lineTo(25.803595, 45.569057);
    ctx.lineTo(39.286594, 35.986019);

    vo->Center();
    vo->Scale(0.01f);
    //vo->_solid = true;
    return vo;
}

VectorObject* Assets::Airport(float w, float l)
{
    auto vo = new VectorObject();

    //vo->moveTo(0, 0);
    //vo->lineTo(w, 0);

    vo->moveTo(0, 0);
    vo->lineTo(w / 2, -l / 4);
    vo->lineTo(w, 0);

    vo->moveTo(0, -l / 4);
    vo->lineTo(w / 2, -l / 2);
    vo->lineTo(w, -l / 4);

    vo->moveTo(0, -l / 2);
    vo->lineTo(w / 2, -3 * l / 4);
    vo->lineTo(w, -l / 2);

    vo->moveTo(0, -3 * l / 4);
    vo->lineTo(w / 2, -l);
    vo->lineTo(w, -3 * l / 4);

    //vo->moveTo(0, -l);
    //vo->lineTo(w, -l);

    vo->Center(w / 2, 0);
    //vo->Scale(0.1f);
    return vo;
}

VectorObject* Assets::Tokyo()
{
    auto  vo  = new VectorObject();
    auto& ctx = *vo;

    // #path75
    ctx.beginPath();
    ctx.moveTo(-235.128100, 279.563930);
    ctx.lineTo(-229.584520, 268.873150);
    ctx.lineTo(-199.383070, 255.776940);
    ctx.lineTo(-168.112530, 257.647820);
    ctx.lineTo(-151.274550, 264.062290);
    ctx.lineTo(-139.514690, 279.563930);
    ctx.stroke();

    // #path77
    ctx.beginPath();
    ctx.moveTo(-122.142170, 280.098470);
    ctx.lineTo(-118.934940, 268.338610);
    ctx.lineTo(-130.962070, 264.596830);
    ctx.lineTo(-138.445620, 231.188140);
    ctx.lineTo(-102.096960, 203.392110);
    ctx.lineTo(-114.391360, 182.812360);
    ctx.lineTo(-99.691534, 176.397890);
    ctx.lineTo(-89.000753, 184.683240);
    ctx.lineTo(-75.102737, 171.587040);
    ctx.lineTo(-49.979402, 194.572220);
    ctx.lineTo(-65.748304, 217.824670);
    ctx.lineTo(-88.198944, 227.713640);
    ctx.lineTo(-88.198944, 240.809840);
    ctx.lineTo(-101.027880, 242.948000);
    ctx.lineTo(-103.967850, 258.716900);
    ctx.lineTo(-94.613413, 268.338610);
    ctx.lineTo(-94.880682, 279.296660);
    ctx.stroke();

    // #path79
    ctx.beginPath();
    ctx.moveTo(7.216277, 279.563930);
    ctx.lineTo(10.690781, 255.776940);
    ctx.lineTo(5.879930, 238.404420);
    ctx.lineTo(18.708867, 202.055760);
    ctx.lineTo(64.946495, 163.836220);
    ctx.lineTo(97.286108, 165.439840);
    ctx.lineTo(98.622456, 154.481790);
    ctx.lineTo(93.544335, 154.214520);
    ctx.lineTo(57.730218, 93.811604);
    ctx.lineTo(40.090429, 47.841245);
    ctx.lineTo(38.754082, 22.183371);
    ctx.lineTo(47.841245, 9.354434);
    ctx.stroke();

    // #path85
    ctx.beginPath();
    ctx.moveTo(28.597838, 101.295150);
    ctx.lineTo(1.069078, 56.126599);
    ctx.lineTo(-10.156242, 54.522984);
    ctx.lineTo(5.078121, 72.162772);
    ctx.lineTo(1.069078, 77.775433);
    ctx.lineTo(-28.063300, 74.568198);
    ctx.lineTo(-24.856066, 82.853554);
    ctx.lineTo(0.534539, 89.802561);
    ctx.lineTo(-1.603617, 98.889725);
    ctx.lineTo(26.192414, 105.036920);
    ctx.lineTo(28.597838, 101.295150);
    ctx.stroke();

    // #path202
    ctx.beginPath();
    ctx.moveTo(-114.124090, 167.845260);
    ctx.lineTo(-121.874900, 151.541820);
    ctx.stroke();

    // #path204
    ctx.beginPath();
    ctx.moveTo(-121.874900, 151.541820);
    ctx.lineTo(-116.529510, 145.661890);
    ctx.lineTo(-114.391360, 135.505650);
    ctx.lineTo(-120.271290, 127.220300);
    ctx.lineTo(-135.238380, 97.553378);
    ctx.lineTo(-131.763880, 87.664405);
    ctx.lineTo(-139.247420, 65.213765);
    ctx.lineTo(-154.749060, 39.021351);
    ctx.lineTo(-143.256470, 22.183371);
    ctx.lineTo(-141.652850, 9.621703);
    ctx.stroke();

    // #path206
    ctx.beginPath();
    ctx.moveTo(-235.215270, 186.286860);
    ctx.lineTo(-225.575480, 183.346900);
    ctx.lineTo(-204.461190, 171.587040);
    ctx.lineTo(-182.812360, 170.785230);
    ctx.lineTo(-164.905300, 175.596080);
    ctx.lineTo(-145.661890, 169.716150);
    ctx.lineTo(-139.514690, 171.854300);
    ctx.lineTo(-114.124090, 167.845260);
    ctx.lineTo(-114.391360, 182.812360);
    ctx.stroke();

    // #path208
    ctx.beginPath();
    ctx.moveTo(-114.124090, 167.845260);
    ctx.lineTo(-83.655362, 161.698060);
    ctx.lineTo(-64.144687, 171.854310);
    ctx.lineTo(-42.495855, 179.605120);
    ctx.lineTo(-21.648832, 166.776190);
    ctx.lineTo(0.267270, 138.980150);
    ctx.lineTo(30.735996, 128.289370);
    ctx.lineTo(44.099472, 110.115050);
    ctx.stroke();

    // #path210
    ctx.beginPath();
    ctx.moveTo(-121.874900, 151.541820);
    ctx.lineTo(-145.929160, 149.670930);
    ctx.lineTo(-166.508920, 126.685760);
    ctx.lineTo(-185.752320, 107.442350);
    ctx.lineTo(-189.226830, 81.784475);
    ctx.lineTo(-218.893740, 59.333835);
    ctx.lineTo(-230.326300, 34.745039);
    ctx.lineTo(-235.048000, 23.786988);
    ctx.stroke();

    // #path212
    ctx.beginPath();
    ctx.moveTo(-75.102735, 171.587040);
    ctx.lineTo(-83.655362, 161.698060);
    ctx.lineTo(-85.800595, 150.812500);
    ctx.lineTo(-93.738095, 140.040180);
    ctx.lineTo(-96.194941, 130.023810);
    ctx.lineTo(-120.271290, 127.220300);
    ctx.stroke();

    // #path214
    ctx.beginPath();
    ctx.moveTo(-120.271290, 127.220300);
    ctx.lineTo(-137.394340, 134.748510);
    ctx.lineTo(-145.929160, 149.670930);
    ctx.lineTo(-145.661890, 169.716150);
    ctx.stroke();

    // #path216
    ctx.beginPath();
    ctx.moveTo(-145.661890, 169.716150);
    ctx.lineTo(-140.040180, 182.373510);
    ctx.lineTo(-130.590770, 192.767860);
    ctx.lineTo(-116.605650, 201.461310);
    ctx.lineTo(-107.534230, 208.453870);
    ctx.stroke();

    // #path218
    ctx.beginPath();
    ctx.moveTo(-114.391360, 182.812360);
    ctx.lineTo(-115.849700, 195.035710);
    ctx.lineTo(-120.385420, 207.130950);
    ctx.lineTo(-146.843750, 219.793150);
    ctx.lineTo(-168.766370, 211.099700);
    ctx.lineTo(-186.909230, 230.187500);
    ctx.lineTo(-205.052080, 234.345240);
    ctx.lineTo(-218.092260, 246.062500);
    ctx.lineTo(-235.492640, 244.107770);
    ctx.stroke();

    vo->Center();
    vo->Scale(0.066f);
    //vo->_solid = true;
    return vo;
}

VectorObject* Assets::Inlay()
{
    float w = 21.95;
    float h = 17.95;

    auto vo = new VectorObject();

    for(int x = 1; x < 5; x++)
        for(int y = 1; y < 5; y++)
        {
            float x1 = w * (x / 5.0);
            float y1 = h * (y / 5.0);
            vo->AddLine(x1 - 0.1f, y1, x1 + 0.1f, y1);
            vo->AddLine(x1, y1 - 0.1f, x1, y1 + 0.1f);
        }

    vo->Center();
    return vo;
}

VectorObject* Assets::Outline()
{
    float w = 21.95;
    float h = 17.95;
    //outline = VectorObject::Rectangle(21.95, 17.95);

    auto vo = new VectorObject();
    vo->lineTo(w, 0);
    vo->lineTo(w, h);
    vo->lineTo(0, h);
    vo->lineTo(0, 0);

    // add tings?
    for(int i = 1; i < 5; i++)
    {
        float w1 = w * (i / 5.0);
        vo->AddLine(w1, 0, w1, 0.1f);
        vo->AddLine(w1, h, w1, h - 0.1f);

        float h1 = h * (i / 5.0);
        vo->AddLine(0, h1, 0.1f, h1);
        vo->AddLine(w - 0.1f, h1, w, h1);
    }
    vo->Center();

    return vo;
}

VectorObject* Assets::SF()
{
    auto  vo  = new VectorObject();
    auto& ctx = *vo;

    // #path109
    ctx.beginPath();
    ctx.moveTo(17.008928, 246.818450);
    ctx.lineTo(24.946428, 192.011900);
    ctx.lineTo(6.425595, 151.568450);
    ctx.lineTo(4.157738, 130.779760);
    ctx.lineTo(-21.544643, 104.699400);
    ctx.lineTo(-13.607143, 69.169643);
    ctx.lineTo(-13.229167, 38.175595);
    ctx.lineTo(-18.142857, -17.008929);
    ctx.lineTo(-4.157738, -28.348214);
    ctx.lineTo(29.482143, -24.190476);
    ctx.lineTo(40.821429, 10.961310);
    ctx.lineTo(31.372024, 20.410714);
    ctx.lineTo(33.261905, 54.806548);
    ctx.lineTo(42.333333, 75.217262);
    ctx.lineTo(65.389881, 84.288691);
    ctx.lineTo(84.666667, 111.880950);
    ctx.lineTo(111.502980, 127.000000);
    ctx.lineTo(142.497020, 152.324400);
    ctx.lineTo(182.184520, 160.639880);
    ctx.lineTo(207.886900, 146.654760);
    ctx.stroke();

    // #path113
    ctx.beginPath();
    ctx.moveTo(207.886900, 146.654760);
    ctx.lineTo(198.815470, 132.291660);
    ctx.lineTo(150.812500, 106.211310);
    ctx.lineTo(103.943450, 20.788690);
    ctx.lineTo(91.848214, 21.922619);
    ctx.lineTo(78.619048, 3.023810);
    ctx.lineTo(85.800595, -2.267857);
    ctx.lineTo(49.892857, -17.386905);
    ctx.lineTo(56.696429, -38.931548);
    ctx.lineTo(65.011905, -42.333333);
    ctx.lineTo(59.342262, -66.901786);
    ctx.stroke();

    // #path115
    ctx.beginPath();
    ctx.moveTo(-62.933221, -66.653600);
    ctx.lineTo(-24.946428, -36.285715);
    ctx.lineTo(-4.913691, -37.419643);
    ctx.lineTo(-3.401786, -53.294643);
    ctx.lineTo(-16.714797, -66.607654);
    ctx.stroke();

    // #path220
    ctx.beginPath();
    ctx.moveTo(158.758100, -66.015573);
    ctx.lineTo(179.872390, -37.952272);
    ctx.lineTo(208.470230, 22.183371);
    ctx.lineTo(227.980910, 75.637276);
    ctx.lineTo(199.383070, 108.511430);
    ctx.lineTo(222.635520, 155.283600);
    ctx.lineTo(242.146190, 192.968600);
    ctx.lineTo(219.161010, 207.133880);
    ctx.lineTo(209.539310, 209.806580);
    ctx.lineTo(183.079630, 209.539310);
    ctx.lineTo(171.052500, 201.253950);
    ctx.lineTo(149.403670, 202.323030);
    ctx.lineTo(110.382310, 171.319770);
    ctx.lineTo(84.724440, 152.343630);
    ctx.lineTo(66.817382, 141.385580);
    ctx.lineTo(64.946495, 125.082140);
    ctx.lineTo(46.772167, 114.124090);
    ctx.lineTo(12.027129, 70.024616);
    ctx.lineTo(14.967094, 63.342878);
    ctx.lineTo(-3.741773, 32.072343);
    ctx.lineTo(-2.405426, 18.441597);
    ctx.lineTo(6.147199, 15.768902);
    ctx.lineTo(9.621703, 8.285355);
    ctx.stroke();

    // #path222
    ctx.beginPath();
    ctx.moveTo(9.621703, 8.285355);
    ctx.lineTo(23.519718, 6.949008);
    ctx.lineTo(29.132379, -0.267270);
    ctx.lineTo(23.786988, -14.699824);
    ctx.lineTo(29.482143, -24.190476);
    ctx.stroke();

    // #path224
    ctx.beginPath();
    ctx.moveTo(21.381562, -9.087164);
    ctx.lineTo(23.519718, 6.949008);
    ctx.lineTo(29.132379, 33.675960);
    ctx.lineTo(20.045215, 44.901281);
    ctx.lineTo(25.657875, 65.481034);
    ctx.lineTo(40.090429, 80.180858);
    ctx.lineTo(63.877417, 95.949760);
    ctx.lineTo(82.051745, 118.667670);
    ctx.lineTo(111.502980, 127.000000);
    ctx.stroke();

    // #path226
    ctx.beginPath();
    ctx.moveTo(111.502980, 127.000000);
    ctx.lineTo(151.007280, 165.439840);
    ctx.lineTo(233.593570, 196.175830);
    ctx.lineTo(267.536800, 245.620700);
    ctx.stroke();

    // #path228
    ctx.beginPath();
    ctx.moveTo(277.693040, 21.916101);
    ctx.lineTo(208.470230, 22.183371);
    ctx.lineTo(181.208740, 21.381562);
    ctx.lineTo(138.712880, 29.132379);
    ctx.lineTo(122.943980, 10.423512);
    ctx.lineTo(107.709620, 16.303441);
    ctx.stroke();

    vo->Center();
    //vo->Center(50.0f, 90.0f);

    vo->Scale(0.057f);
    //vo->_solid = true;
    return vo;
}

VectorObject* Assets::London()
{
    auto  vo  = new VectorObject();
    auto& ctx = *vo;

    // #path139
    ctx.beginPath();
    ctx.moveTo(260.458670, 134.971110);
    ctx.lineTo(244.284350, 148.869130);
    ctx.stroke();

    // #path141
    ctx.beginPath();
    ctx.moveTo(244.284350, 148.869130);
    ctx.lineTo(215.953780, 143.256460);
    ctx.lineTo(212.479270, 151.007280);
    ctx.lineTo(177.734240, 154.749060);
    ctx.lineTo(176.130620, 167.043450);
    ctx.lineTo(160.628990, 169.983420);
    ctx.lineTo(155.016330, 163.568950);
    ctx.lineTo(148.601860, 164.370760);
    ctx.lineTo(146.196430, 167.845260);
    ctx.lineTo(136.307460, 159.559910);
    ctx.lineTo(128.289370, 159.292640);
    ctx.lineTo(125.349410, 152.878170);
    ctx.lineTo(117.865860, 150.205470);
    ctx.lineTo(110.916850, 151.809090);
    ctx.lineTo(104.235120, 156.887210);
    ctx.lineTo(96.484299, 155.283600);
    ctx.lineTo(91.673448, 152.610900);
    ctx.lineTo(92.207987, 156.619940);
    ctx.lineTo(92.742526, 159.559910);
    ctx.lineTo(90.604370, 159.827180);
    ctx.lineTo(87.397136, 158.758100);
    ctx.lineTo(86.862596, 157.154480);
    ctx.lineTo(86.328057, 154.214520);
    ctx.lineTo(84.457171, 152.878170);
    ctx.lineTo(82.586284, 153.679980);
    ctx.lineTo(80.715397, 155.016330);
    ctx.lineTo(75.102737, 152.878170);
    ctx.lineTo(70.291886, 152.878170);
    ctx.lineTo(68.955538, 156.619940);
    ctx.lineTo(67.619190, 159.827180);
    ctx.lineTo(63.877417, 160.361720);
    ctx.lineTo(59.066566, 161.163530);
    ctx.lineTo(57.462948, 164.638030);
    ctx.lineTo(54.790253, 166.508920);
    ctx.lineTo(50.781210, 164.370760);
    ctx.lineTo(48.643054, 159.827180);
    ctx.lineTo(46.772167, 158.758100);
    ctx.lineTo(44.901281, 159.292640);
    ctx.lineTo(43.832203, 163.301680);
    ctx.lineTo(41.159507, 165.172570);
    ctx.lineTo(39.288621, 160.094450);
    ctx.lineTo(36.081386, 159.827180);
    ctx.lineTo(31.270535, 163.301680);
    ctx.lineTo(33.943230, 169.983420);
    ctx.lineTo(29.666918, 172.923380);
    ctx.stroke();

    // #path143
    ctx.beginPath();
    ctx.moveTo(260.665900, 182.010550);
    ctx.lineTo(252.035160, 178.803310);
    ctx.lineTo(234.128110, 172.121580);
    ctx.stroke();

    // #path145
    ctx.beginPath();
    ctx.moveTo(234.128110, 172.121580);
    ctx.lineTo(233.860830, 182.277820);
    ctx.lineTo(225.308210, 192.968600);
    ctx.lineTo(210.875660, 191.899520);
    ctx.lineTo(195.106760, 182.545090);
    ctx.lineTo(212.479270, 180.406930);
    ctx.lineTo(215.151970, 175.328810);
    ctx.lineTo(229.317250, 175.061540);
    ctx.lineTo(230.386330, 167.043450);
    ctx.lineTo(225.575480, 163.836220);
    ctx.stroke();

    // #path147
    ctx.beginPath();
    ctx.moveTo(225.575480, 163.836220);
    ctx.lineTo(184.415970, 159.827170);
    ctx.lineTo(181.208740, 170.785230);
    ctx.lineTo(163.568950, 173.190650);
    ctx.lineTo(157.154480, 172.388850);
    ctx.lineTo(152.610900, 167.043450);
    ctx.lineTo(147.532780, 170.517960);
    ctx.lineTo(134.436570, 161.965330);
    ctx.lineTo(128.556640, 161.965330);
    ctx.lineTo(123.745790, 155.550870);
    ctx.lineTo(116.529510, 152.343630);
    ctx.lineTo(110.916850, 153.679980);
    ctx.lineTo(105.304190, 156.619940);
    ctx.stroke();

    // #path164
    ctx.beginPath();
    ctx.moveTo(122.943980, 214.884700);
    ctx.lineTo(122.943980, 197.512180);
    ctx.stroke();

    // #path166
    ctx.beginPath();
    ctx.moveTo(122.943980, 197.512180);
    ctx.lineTo(138.980150, 177.734230);
    ctx.lineTo(138.980150, 169.716150);
    ctx.lineTo(145.127350, 160.094450);
    ctx.lineTo(148.334590, 136.307460);
    ctx.lineTo(141.118310, 118.133130);
    ctx.lineTo(116.796780, 99.424264);
    ctx.lineTo(104.235120, 96.751569);
    ctx.lineTo(96.484299, 99.424264);
    ctx.lineTo(71.360964, 96.217030);
    ctx.lineTo(49.177593, 98.622456);
    ctx.lineTo(39.021351, 87.397136);
    ctx.lineTo(16.837980, 88.466214);
    ctx.lineTo(8.819894, 90.604370);
    ctx.lineTo(6.414469, 98.889725);
    ctx.lineTo(-2.138156, 103.433310);
    ctx.lineTo(-10.423512, 134.436570);
    ctx.lineTo(-4.543582, 142.721930);
    ctx.lineTo(-3.474504, 157.956290);
    ctx.lineTo(-12.294398, 175.596080);
    ctx.lineTo(-12.294398, 187.088670);
    ctx.lineTo(0.801809, 207.668420);
    ctx.lineTo(17.639789, 215.953780);
    ctx.lineTo(29.399648, 213.548350);
    ctx.lineTo(33.675960, 219.161010);
    ctx.lineTo(39.021351, 219.428280);
    ctx.lineTo(48.108515, 230.119060);
    ctx.lineTo(60.670183, 226.377290);
    ctx.lineTo(82.853554, 231.455410);
    ctx.lineTo(91.138909, 225.842750);
    ctx.lineTo(98.355186, 226.644560);
    ctx.lineTo(118.400400, 220.497360);
    ctx.lineTo(122.943980, 214.884700);
    ctx.stroke();

    // #path168
    ctx.beginPath();
    ctx.moveTo(70.291885, 152.878170);
    ctx.lineTo(57.730217, 144.058270);
    ctx.stroke();

    // #path170
    ctx.beginPath();
    ctx.moveTo(57.730217, 144.058270);
    ctx.lineTo(45.970359, 124.013060);
    ctx.lineTo(39.021351, 113.055010);
    ctx.lineTo(26.459683, 108.244160);
    ctx.lineTo(15.234363, 79.379050);
    ctx.stroke();

    // #path172
    ctx.beginPath();
    ctx.moveTo(15.234363, 79.379050);
    ctx.lineTo(11.492589, 74.568197);
    ctx.lineTo(10.423512, 44.901281);
    ctx.lineTo(-3.474504, 22.450640);
    ctx.lineTo(-13.363476, -3.474504);
    ctx.stroke();

    // #path176
    ctx.beginPath();
    ctx.moveTo(75.102735, 152.878170);
    ctx.lineTo(87.129865, 140.316500);
    ctx.lineTo(100.493340, 125.616680);
    ctx.lineTo(116.796780, 99.424264);
    ctx.lineTo(121.607640, 90.069831);
    ctx.lineTo(120.004020, 77.775433);
    ctx.lineTo(129.625720, 51.048480);
    ctx.lineTo(128.289370, 44.901281);
    ctx.lineTo(135.238380, 28.330570);
    ctx.lineTo(130.962070, -2.939965);
    ctx.stroke();

    // #path178
    ctx.beginPath();
    ctx.moveTo(70.291885, 152.878170);
    ctx.lineTo(53.988443, 157.421750);
    ctx.lineTo(30.468726, 157.154480);
    ctx.lineTo(-3.474504, 157.956290);
    ctx.stroke();

    // #path180
    ctx.beginPath();
    ctx.moveTo(-3.474504, 157.956290);
    ctx.lineTo(-18.976136, 159.025370);
    ctx.lineTo(-22.450640, 153.145440);
    ctx.lineTo(-37.952273, 151.809090);
    ctx.lineTo(-72.162772, 176.932430);
    ctx.lineTo(-80.475257, 182.812360);
    ctx.lineTo(-94.442350, 178.536040);
    ctx.stroke();

    // #path182
    ctx.beginPath();
    ctx.moveTo(70.291885, 152.878170);
    ctx.lineTo(83.388090, 162.767140);
    ctx.lineTo(102.898770, 168.914340);
    ctx.lineTo(138.980150, 177.734230);
    ctx.stroke();

    // #path188
    ctx.beginPath();
    ctx.moveTo(138.980150, 177.734230);
    ctx.lineTo(168.112530, 183.881430);
    ctx.lineTo(180.406930, 187.623210);
    ctx.lineTo(192.166790, 209.272040);
    ctx.lineTo(200.184880, 209.272040);
    ctx.lineTo(214.617430, 203.659380);
    ctx.lineTo(231.722680, 216.221050);
    ctx.lineTo(260.467720, 216.755590);
    ctx.stroke();

    // #path190
    ctx.beginPath();
    ctx.moveTo(67.619189, 159.827180);
    ctx.lineTo(68.688267, 172.656110);
    ctx.lineTo(70.826425, 201.521220);
    ctx.lineTo(60.670183, 226.377290);
    ctx.stroke();

    // #path192
    ctx.beginPath();
    ctx.moveTo(60.670183, 226.377290);
    ctx.lineTo(70.024616, 232.524480);
    ctx.lineTo(66.015573, 267.269530);
    ctx.lineTo(52.652097, 287.047470);
    ctx.stroke();

    vo->Center();
    vo->Scale(0.062f);
    //vo->_solid = true;
    return vo;
}