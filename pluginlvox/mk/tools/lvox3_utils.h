#ifndef LVOX3_UTILS_H
#define LVOX3_UTILS_H

#include "mk/tools/lvox3_gridtype.h"
#include <QString>

#define DEF_SearchInResult      "r"
#define DEF_SearchInGrid        "grid"
#define DEF_SearchInGroup       "gr"
#define DEF_Nt "theoretical"
#define DEF_Nb "before"
#define DEF_Ni "hits"
#define DEF_Density "density"
#define DEF_LVOX_GRD_NT "LVOX_GRD_NT"
#define DEF_LVOX_GRD_NTA "LVOX_GRD_NTA"
#define DEF_LVOX_GRD_NI "LVOX_GRD_NI"
#define DEF_LVOX_GRD_NB "LVOX_GRD_NB"
#define DEF_LVOX_GRD_DENSITY "LVOX_GRD_DENSITY"

class CT_InResultModelGroupToCopy;

enum LVOXGridId {
    Theoretic = 0,
    Hits,
    Blocked,
    Density
};

struct LVOXGridDefinitionStruct {
    LVOXGridId id;
    QString name;
    QString type;
    QString disp;
    QString desc;
    QString attr;
    QString attr_label;
};

class LVOX3_Utils
{
public:
    LVOX3_Utils();

    static void requireGrid(CT_InResultModelGroupToCopy *model, LVOXGridId id);

    static const LVOXGridDefinitionStruct LVOXGridDefinitions[];

};

#endif // LVOX3_UTILS_H