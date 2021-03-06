#include <ygo/deck/DeckSet.h>
#include <ygo/deck/c/DeckSet.h>

#include <ygo/deck/User.h>
#include <ygo/deck/Format.h>
#include <ygo/data/cpp/DataTypes.h>
#include <ygo/data/c/DataUtils.h>
#include <ygo/data/Serialize.h>

#include <zephyr/cstring.h>

#include <stdexcept>

extern "C" {

#define C_CAST(p) reinterpret_cast<DECKSET_THIS>(p)
#define CPP_CAST(p) reinterpret_cast<ygo::deck::DeckSet*>(p)

DECKSET_THIS DECKSET_NAME(new)(const char* name, USER_THIS u, FORMAT_THIS f)
{
    try {
        return C_CAST(new ygo::deck::DeckSet(name,
                    *reinterpret_cast<ygo::deck::User*>(u),
                    *reinterpret_cast<ygo::deck::Format*>(f)));
    } catch (std::runtime_error& e) {
        return nullptr;
    }
}

DECKSET_THIS DECKSET_NAME(create)(const char* name, USER_THIS u, FORMAT_THIS f)
{
    try {
        return C_CAST(new ygo::deck::DeckSet(name,
                    *reinterpret_cast<ygo::deck::User*>(u),
                    *reinterpret_cast<ygo::deck::Format*>(f),
                    true));
    } catch (std::runtime_error& e) {
        return nullptr;
    }
}

void DECKSET_NAME(delete)(DECKSET_THIS p)
{
    delete CPP_CAST(p);
}

char* DECKSET_NAME(name)(DECKSET_THIS p)
{
    return zephyr::cstring::make_copy(CPP_CAST(p)->name());
}

void DECKSET_NAME(delete_name)(char* name)
{
    zephyr::cstring::delete_copy(name);
}

FORMAT_THIS DECKSET_NAME(format)(DECKSET_THIS p)
{
    return reinterpret_cast<FORMAT_THIS>(new
            ygo::deck::Format(CPP_CAST(p)->format()));
}

void DECKSET_NAME(delete_format)(FORMAT_THIS f)
{
    delete reinterpret_cast<ygo::deck::Format*>(f);
}

ygo_deck_DeckError
DECKSET_NAME(addCard)(DECKSET_THIS p,
        ygo_data_DeckType deckType, const char* name)
{
    return static_cast<ygo_deck_DeckError>(
            CPP_CAST(p)->addCard(static_cast<ygo::data::DeckType>(deckType),
                name));
}

void
DECKSET_NAME(deleteCard)(DECKSET_THIS p,
        ygo_data_DeckType deckType, const char* name)
{
    CPP_CAST(p)->deleteCard(static_cast<ygo::data::DeckType>(deckType), name);
}

ygo_deck_CardMap DECKSET_NAME(cards)(DECKSET_THIS p)
{
    auto cards = CPP_CAST(p)->cards();
    auto main = cards[ygo::data::DeckType::MAIN];
    auto side = cards[ygo::data::DeckType::SIDE];
    auto extra = cards[ygo::data::DeckType::EXTRA];

    ygo_deck_CardMap ret;
    ret.main_count = main.size();
    ret.side_count = side.size();
    ret.extra_count = extra.size();

    ret.main = new ygo_data_StaticCardData*[ret.main_count];
    ret.side = new ygo_data_StaticCardData*[ret.side_count];
    ret.extra = new ygo_data_StaticCardData*[ret.extra_count];

    for (auto i = 0; i < ret.main_count; i++) {
        ret.main[i] = ygo_data_static_card_data_to_c(SCD_CAST(&main[i]));
    }

    for (auto i = 0; i < ret.side_count; i++) {
        ret.side[i] = ygo_data_static_card_data_to_c(SCD_CAST(&side[i]));
    }

    for (auto i = 0; i < ret.extra_count; i++) {
        ret.extra[i] = ygo_data_static_card_data_to_c(SCD_CAST(&extra[i]));
    }

    return ret;
}

void DECKSET_NAME(delete_cards)(ygo_deck_CardMap c)
{
    for (auto i = 0; i < c.main_count; i++) {
        ygo_data_delete_static_card_data(c.main[i]);
    }
    delete[] c.main;

    for (auto i = 0; i < c.side_count; i++) {
        ygo_data_delete_static_card_data(c.side[i]);
    }
    delete[] c.side;

    for (auto i = 0; i < c.extra_count; i++) {
        ygo_data_delete_static_card_data(c.extra[i]);
    }
    delete[] c.extra;
}

void DECKSET_NAME(remove)(DECKSET_THIS p)
{
    CPP_CAST(p)->remove();
}

int DECKSET_NAME(validate)(DECKSET_THIS p)
{
    auto v = CPP_CAST(p)->validate();
    if (v) return 1;
    return 0;
}

char* DECKSET_NAME(exportCards)(DECKSET_THIS p)
{
    return zephyr::cstring::make_copy(CPP_CAST(p)->exportCards());
}

void DECKSET_NAME(delete_exportCards)(char* cardMap)
{
    zephyr::cstring::delete_copy(cardMap);
}

}
