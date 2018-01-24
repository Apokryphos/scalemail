#include <iostream>

namespace ScaleMail
{
//	============================================================================
int toTinyDungeonFontIndex(int charCode) {
    int index = (int)charCode;

    if (index >= 48 && index <= 57) {
        return 26 + (index - 48);
    } else if (index >= 65 && index < 91) {
        return index - 65;
    } else {
        switch (index) {
            //  SPACE
            case 32:
                return -1;

            //  !
            case 33:
                return 36;

            //  @
            case 64:
                return 37;
                break;

            //  #
            case 35:
                return 38;

            //  $
            case 36:
                return 39;

            //  %
            case 37:
                return 40;

            //  ^
            case 94:
                return 41;

            //  (
            case 40:
                return 44;

            //  )
            case 41:
                return 45;

            //  -
            case 45:
                return 46;

            //  +
            case 43:
                return 47;

            //  =
            case 61:
                return 48;

            //  :
            case 58:
                return 49;

            //  ;
            case 59:
                return 50;

            //  ,
            case 44:
                return 51;

            //  "
            case 34:
                return 52;

            //  <
            case 60:
                return 53;

            //  >
            case 62:
                return 54;

            //  .
            case 46:
                return 55;
                break;

            //  ?
            case 63:
                return 56;
                break;

            // /
            case 47:
                return 57;
                break;

            // Backslash
            case 92:
                return 58;
                break;

            //  [
            case 91:
                return 59;
                break;

            //  ]
            case 93:
                return 60;
                break;

            //  _
            case 95:
                return 61;
                break;

            //  |
            case 124:
                return 62;
                break;

            //  *
            case 42:
                return 64;
                break;

            default:
                std::cout << "Unknown char code: " << index << std::endl;
                return -1;
        }
    }
}
}