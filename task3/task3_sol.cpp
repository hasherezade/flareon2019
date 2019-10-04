#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


int m = 0;
int c = 0;
int h = 0;

void play()
{
    m -= 2;
    h += 4;
    c -= 1;
}

void clean()
{
    h -= 1;
    c += 6;
}

void feed()
{
    m += 10;
    h += 2;
    c -= 1;
}

bool is_ok() {
    if (m == 72 && h == 30 && c == 0) {
        return true;
    }
    return false;
}

void reset()
{
    m = 0;
    c = 0;
    h = 0;
}


bool check_series(size_t feed_times, size_t play_times, size_t clean_times)
{
    reset();

    for (size_t i = 0; i < feed_times; i++) {
        feed();
    }
    for (size_t i = 0; i < play_times; i++) {
        play();
    }
    for (size_t i = 0; i < clean_times; i++) {
        clean();
    }
    return is_ok();
}

int main()
{
    /* initialize random seed: */
    srand(time(NULL));

    while (true) {
        size_t feed_times = rand() % 10 + 1;
        size_t play_times = rand() % 10 + 1;
        size_t clean_times = rand() % 10 + 1;

        if (check_series(feed_times, play_times, clean_times)) {
            std::cout << "SUCCESS!" << "\n";
            std::cout << "play: " << play_times << ", clean: " << clean_times << ", feed:" << feed_times << "\n";
            break;
        }
    }
    system("pause");
    return 0;
}
