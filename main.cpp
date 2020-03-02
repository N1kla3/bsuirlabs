/*This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "Conveyer.h"

int main() {
    vector<int> one{3,5,6,8};
    vector<int> two{5,9,6,2};
    auto Dx = new Conveyer;
    Dx->calculate(one, two);
}
