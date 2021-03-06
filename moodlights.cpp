/*
 * libhausbus: A RS485 Hausbus library
 *
 * Copyright (c) Ralf Ramsauer, 2016
 *
 * Authors:
 *   Ralf Ramsauer <ralf@binary-kitchen.de>
 *
 * This work is licensed under the terms of the GNU GPL, version 3.  See
 * the LICENSE file in the top-level directory.
 */

#include <cstdlib>

#include "moodlights.h"

#define rand_byte() ((Byte)::rand())

const std::regex Moodlights::_color_regex("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");
const Byte Moodlights::_gamma_correction[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
    0x05, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0A,
    0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0F, 0x0F, 0x10, 0x10,
    0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x18, 0x18, 0x19,
    0x19, 0x1A, 0x1B, 0x1B, 0x1C, 0x1D, 0x1D, 0x1E, 0x1F, 0x20, 0x20, 0x21, 0x22, 0x23, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x32,
    0x33, 0x34, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x44,
    0x45, 0x46, 0x48, 0x49, 0x4A, 0x4B, 0x4D, 0x4E, 0x4F, 0x51, 0x52, 0x53, 0x55, 0x56, 0x57, 0x59,
    0x5A, 0x5C, 0x5D, 0x5F, 0x60, 0x62, 0x63, 0x65, 0x66, 0x68, 0x69, 0x6B, 0x6D, 0x6E, 0x70, 0x72,
    0x73, 0x75, 0x77, 0x78, 0x7A, 0x7C, 0x7E, 0x7F, 0x81, 0x83, 0x85, 0x87, 0x89, 0x8A, 0x8C, 0x8E,
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E, 0xA0, 0xA2, 0xA4, 0xA7, 0xA9, 0xAB, 0xAD, 0xAF,
    0xB1, 0xB4, 0xB6, 0xB8, 0xBA, 0xBD, 0xBF, 0xC1, 0xC4, 0xC6, 0xC8, 0xCB, 0xCD, 0xD0, 0xD2, 0xD5,
    0xD7, 0xDA, 0xDC, 0xDF, 0xE1, 0xE4, 0xE7, 0xE9, 0xEC, 0xEF, 0xF1, 0xF4, 0xF7, 0xF9, 0xFC, 0xFF,
};

Moodlights::Moodlights(const Byte src, const Byte dst) :
    _src(src),
    _dst(dst)
{
    set_all(Color {0, 0, 0});
}

Moodlights::~Moodlights()
{
}

Moodlights::Color Moodlights::rand_color()
{
    return Color {rand_byte(), rand_byte(), rand_byte()};
}

std::experimental::optional<Moodlights::Color> Moodlights::parse_color(const std::string &str)
{
    std::smatch sm;
    Color retval;
    if (!std::regex_match(str, sm, _color_regex))
        return std::experimental::optional<Color>();

    auto fromHex = [] (const std::string hex) -> Byte {
        return (Byte)strtoul(hex.c_str(), nullptr, 16);
    };

    retval[0] = fromHex(sm[1]);
    retval[1] = fromHex(sm[2]);
    retval[2] = fromHex(sm[3]);

    return std::experimental::optional<Color>(retval);
}

std::string Moodlights::color_to_string(const Color &color)
{
    char retval[7];
    snprintf(retval, 7, "%02X%02X%02X", color[0], color[1], color[2]);
    return retval;
}

void Moodlights::set(unsigned int no, const Color &c)
{
    if (no >= MOODLIGHTS_LAMPS)
        throw std::runtime_error("Invalid lamp");

    _lamps[no] = c;
}

void Moodlights::set_all(const Color &c)
{
    for (int i = 0 ; i < MOODLIGHTS_LAMPS ; i++)
        set(i, c);
}

const Moodlights::Color &Moodlights::get(unsigned int no) const
{
    if (no > MOODLIGHTS_LAMPS)
        throw std::runtime_error("Invalid lamp");

    return _lamps[no];
}

void Moodlights::rand(const unsigned int no)
{
    set(no, rand_color());
}

void Moodlights::rand_all()
{
    for (auto &lamp : _lamps)
        lamp = rand_color();
}

Data Moodlights::get_payload() const
{
    Data payload(MOODLIGHTS_LAMPS * 3);

    // assemble packet
    for (int i = 0 ; i < MOODLIGHTS_LAMPS ; i++) {
        payload[i*3] = _gamma_correction[_lamps[i][0]];
        payload[i*3 + 1] = _gamma_correction[_lamps[i][1]];
        payload[i*3 + 2] = _gamma_correction[_lamps[i][2]];
    }

    return payload;
}

void Moodlights::blank(unsigned int no)
{
    set(no, Color {0, 0, 0});
}

void Moodlights::blank_all()
{
    set_all(Color {0, 0, 0});
}

Hausbus &operator <<(Hausbus &h, const Moodlights &m)
{
    h.send(m._src, m._dst, m.get_payload());
    return h;
}
