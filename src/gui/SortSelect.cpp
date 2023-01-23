#include "SortSelect.h"

void SortSelect::applySort(Player* p) {
    if (ptr == 0) {
        p->sort_by_date();
    } else if (ptr == 1) {
        p->sort_by_album();
    } else if (ptr == 2) {
        p->sort_by_random();
    }
}

SortSelect::SortSelect() : SelectView({"Date", "Album", "Random"}) {}
