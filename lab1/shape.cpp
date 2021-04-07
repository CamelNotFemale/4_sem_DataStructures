//========== ���� shape.cpp (���������� ���������) ==========
// ���������� � ������������� ���������� �����
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "screen.h"
#include "shape.h"
// ����� �����
class obl_cross: public shape {
/* nw  n   ne
    \     /
      \ /
   w   c   e
      / \
    /     \
   sw  s   se
*/
    line left_obl;
    line right_obl;
public:
    obl_cross(point a, point b) : left_obl(point(a.x, b.y), point(b.x, a.y)), right_obl(a,b) { };
    obl_cross(point centre, int L)
        : left_obl(point(centre.x-L,centre.y-L), point(centre.x+L,centre.y+L)),
        right_obl(point(centre.x-L,centre.y+L), point(centre.x+L,centre.y-L))
        {};
    point north( ) const { return left_obl.north(); }
    point south( ) const { return left_obl.south(); }
    point east( ) const { return left_obl.east(); }
    point west( ) const { return left_obl.west(); }
    point neast( ) const { return left_obl.neast(); }
    point seast( ) const { return left_obl.seast(); }
    point nwest( ) const { return left_obl.nwest(); }
    point swest( ) const { return left_obl.swest(); }
    void move(int a, int b) { left_obl.move(a,b); right_obl.move(a,b); }
    void draw( ) { left_obl.draw(); right_obl.draw(); }
    void resize(int d) // ���������� ����� ����� � (d) ���
    { left_obl.resize(d); right_obl.resize(d); }
};
class circle : public shape {
protected:
    point c;
    int radius;
public:
    circle(point a, int r) : c(a), radius(r) { }
    void draw();
    point north() const { return point(c.x,c.y+radius); }
    point south() const { return point(c.x,c.y-radius); }
    point east() const { return point(c.x+radius,c.y); }
    point west() const { return point(c.x-radius,c.y); }
    point neast() const { return point(c.x+radius*0.7, c.y+radius*0.7); }
    point seast() const { return point(c.x+radius*0.7, c.y-radius*0.7); }
    point nwest() const { return point(c.x-radius*0.7, c.y+radius*0.7); }
    point swest() const { return point(c.x-radius*0.7, c.y-radius*0.7); }
    void move(int a , int b) { c.x += a; c.y += b; } //�����������
    void resize(int d) { (d>0) ? radius *= d : radius /= -d; std::cout<<radius<<std::endl;} //��������� �������
private:
    circle(const circle &x) { std::cout << "copy constructor\n"; } // ����������� �����������
    circle& operator=(const circle &x) { }// �������� ������������ ������������
    circle(circle &&x) { std::cout << "move constructor\n"; } // ����������� �����������
    circle& operator=(circle &&x) { } // �������� ������������ ������������
};
void circle::draw() {
    int x = 0, y = radius, delta = 1 - 2 * radius, error = 0;
    while (y >= 0) {
        put_point(c.x + x, c.y + y);
        put_point(c.x + x, c.y - y);
        put_point(c.x - x, c.y + y);
        put_point(c.x - x, c.y - y);
        error = 2 * (delta + y) - 1;
        if (delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if (delta > 0 && error > 0) {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }
}

// ������ �������: �������������� �������� - ��������������
class h_circle: public rectangle, public reflectable {
    bool reflected;
public:
    h_circle(point a, point b, bool r=true) : rectangle(a, b), reflected(r) { }
    void draw();
    void flip_horisontally( ) { }; // �������� ������������� (������ �������)
    void flip_vertically( ) { reflected = !reflected; }; // �������� �����������
};
void h_circle :: draw() //�������� ���������� ��� �����������
{ //(�������� ��� �������, ����������� ��������� reflected)
    int x0 = (sw.x + ne.x)/2, y0 = reflected ? sw.y : ne.y;
    int radius = (ne.x - sw.x)/2;
    int x = 0, y = radius, delta = 2 - 2 * radius, error = 0;
    while(y >= 0) { // ���� ���������
        if(reflected) { put_point(x0 + x, y0 + y*0.7); put_point(x0 - x, y0 + y*0.7); }
        else { put_point(x0 + x, y0 - y*0.7); put_point(x0 - x, y0 - y*0.7); }
        error = 2 * (delta + y) - 1;
        if(delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
        error = 2 * (delta - x) - 1;
        if(delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
        ++x; delta += 2 * (x - y); --y;
    }
}
// ������ �������: �������������� ������� ��������������
void down(shape &p, const shape &q) {
    point n = q.south( );
    point s = p.north( );
    p.move(n.x - s.x, n.y - s.y - 1);
}
void centering(shape &p, const shape &q) {
    int delta_x = (q.east().x + q.west().x)/2 - (p.east().x + p.west().x)/2, // (����� ������ q) - (����� ������ p), ���������� ���
        delta_y = (q.east().y + q.west().y)/2 - (p.east().y + p.west().y)/2; // (����� ������ q) - (����� ������ p), ���������� �����
    p.move(delta_x, delta_y);
}
void rightDown(shape &p, shape&q) {
    point n = q.seast();
    point s = p.nwest();
    p.move(n.x - s.x, n.y - s.y);
}

void leftDown(shape &p, shape &q){
    point n = q.swest();
    point s = p.neast();
    p.move(n.x - s.x - 1, n.y - s.y);
}
// C������ ���������������� ������ - ����������
class myshape : public rectangle { // ��� ������ ��������
    int w, h; // ���������������
    line l_eye; // ����� ���� � ��� ������ �������� �����
    line r_eye; // ������ ����
    line mouth; // ���
public:
    myshape(point, point);
    void draw( );
    void move(int, int);
    void resize(int) { }
};
myshape :: myshape(point a, point b)
   :rectangle(a, b), //������������� �������� ������
    w(neast( ).x - swest( ).x + 1), // ������������� ������
    h(neast( ).y - swest( ).y + 1), // - ������ � ������� ����������!
    l_eye(point(swest( ).x + 2, swest( ).y + h * 3 / 4), 2),
    r_eye(point(swest( ).x + w - 4, swest( ).y + h * 3 / 4), 2),
    mouth(point(swest( ).x + 2, swest( ).y + h / 4), w - 4)
{ }
void myshape :: draw( )
{
    rectangle :: draw( ); //������ ���� (����� � ��� �������� ����!)
    int a = (swest( ).x + neast( ).x) / 2;
    int b = (swest( ).y + neast( ).y) / 2;
    put_point(point(a, b)); // ��� � ���������� ������ �� �������!
}
void myshape :: move(int a, int b)
{
    rectangle :: move(a, b);
    l_eye.move(a, b); r_eye.move(a, b);
    mouth.move(a, b);
}
int main( )
{
    setlocale(LC_ALL, "Rus");
    screen_init( );
    //== 1.���������� ������ ����� ==
    rectangle hat(point(0, 0), point(14, 5));
    line brim(point(0,15),17);
    circle emblem(point(60, 15), 2), whisker_left(point(70,10), 10), whisker_right(point(70,20), 8);
    //circle emb_copy = emblem;
    myshape face(point(15,10), point(27,18));
    //h_circle beard(point(40,10), point(50,20));
    shape_refresh( );
    std::cout << "=== Generated... ===\n";
    std::cin.get(); //�������� �������� �����
    //== 2.���������� � ������ ==
    hat.rotate_right( );
    brim.resize(2);
    face.resize(2);
    //beard.flip_vertically();
    whisker_left.resize(-5);
    whisker_right.resize(-4);
    shape_refresh( );
    std::cout << "=== Prepared... ===\n";
    std::cin.get(); //�������� ��������� ���������/���������
    //== 3.������ ����������� ==
    // face.move(0, -10); // ���� - � �������� ���������
    up(brim, face);
    up(hat, brim);
    //down(beard, face);
    centering(emblem, hat);
    rightDown(whisker_left, face);
    leftDown(whisker_right, face);
    shape_refresh( );
    std::cout << "=== Ready! ===\n";
    std::cin.get(); //�������� ���������
    screen_destroy( );
    return 0;
}
