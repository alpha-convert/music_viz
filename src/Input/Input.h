#pragma once
/***
 * @Author Joseph Cutler
 * @Date September 23, 2016
 * @Copyright WTFPL
 */
template<typename T>
class Input {
public:
        Input(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char code, T startingState) : 
                startingState(startingState), x(x), y(y), width(w), height(h), code(code), state(startingState){};
        ~Input(){};

        void setState(T newState){
                state = newState;
        }

        //THIS MIGHT BE BAD. MAYBE NOT CONST REFS?
        T getState(void){
                return state;
        }

        T getStartingState(void){
                return startingState;
        }

        char getCode(void){
                return code;
        }

        bool mouseInside(unsigned int mx, unsigned int my) const{
                auto inx = mx > x && (mx < (mx + width));
                auto iny = my > y && my < (my + height);
                return inx && iny;
        }

        unsigned int getHeight() const{
                return height;
        }

        unsigned int getWidth() const{
                return width;
        }

        unsigned int getX() const{
                return x;
        }

        unsigned int getY() const{
                return y;
        }


protected:
        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
        char code;
        T state;
        const T startingState;
};
