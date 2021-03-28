#include "ll_cycle.h"

bool ll_has_cycle(node *head) {
    /* Ваш код должен быть написан только внутри этой функции */

    if (!head)
    {
        return false;
    }

    node* rabbit = head;
    node* turtle = head;

    for (;;)
    {
        for (int i = 0; i < 2; i++)
        {
            if (!rabbit->next)
            {
                return false;
            }

            rabbit = rabbit->next;
        }

        turtle = turtle->next;

        if (turtle == rabbit)
        {
            return true;
        }
    }
}