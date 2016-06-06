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
    if (no > MOODLIGHTS_LAMPS)
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
        payload[i*3] = _lamps[i][0];
        payload[i*3 + 1] = _lamps[i][1];
        payload[i*3 + 2] = _lamps[i][2];
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
