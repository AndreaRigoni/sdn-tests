



///
/// \brief The nConsole class
///
class nConsole {
public:
    nConsole() {
        initscr();
        signal(SIGWINCH, nConsole::handle_winch);

    }

    ~nConsole() { endwin(); }

protected:
    // SIGWINCH is called when the window is resized.
    static void handle_winch(int sig){
        signal(SIGWINCH, SIG_IGN);

        // Reinitialize the window to update data structures.
        endwin();
        initscr();
        refresh();
        clear();

        char tmp[128];
        sprintf(tmp, "%dx%d", COLS, LINES);

        // Approximate the center
        int x = COLS / 2 - strlen(tmp) / 2;
        int y = LINES / 2 - 1;

        mvaddstr(y, x, tmp);
        refresh();

        signal(SIGWINCH, nConsole::handle_winch);
    }

};


///
/// \brief The ConsolePlotter class
///
class ConsolePlotter {
    std::vector<Vector2d> bounds;
    const char *symbl, *head, *tail;
public:
    ConsolePlotter() :
        symbl("+oxv|"),
        head(": ["),
        tail(" ]")
    {}

    void print_line(const char *name, int count, ... ) {
        int tw = COLS - strlen(name) - strlen(head) - strlen(tail);
        std::cout << "tw: " << tw << "\n";

        va_list ap;
        va_start (ap, count);
        count = std::min(count,(int)strlen(symbl));
        std::vector<int> cursors(count);

        for(int i=0; i<count; ++i) {
            double val = va_arg(ap, double);
            if (bounds.size() <= i) {
                bounds.push_back(Vector2d(0,val));
            }
            else {
                bounds[i] = Vector2d(0, std::max(val,bounds[i](1)));
            }
            cursors[i] = (int)(bounds[i](1)/val) * tw;
        }

        std::cout << name << head;
        for(int i=0; i<tw; ++i) {
            for(int j=0; j<count; ++j) {
                if(i==j) std::cout << symbl[j];
                else std::cout << '-';
            }
        }
        std::cout << tail << std::endl;
        va_end (ap);
    }

};
