#ifndef __LAB3_H
#define __LAB3_H

#define HEIGHT 5
#define WIDTH  5

class Table {
public:
    int x, y;
    int table[HEIGHT][WIDTH];

    Table (void);
    /* There's nothing allocate memeory for.  */
    ~Table (void);

    virtual void add (int val);
    void del (int val);
    void show (void);
};

class SortedTable: public Table {
public:
    SortedTable (void);
    ~SortedTable (void);
    virtual void add (int val);
};

#endif
