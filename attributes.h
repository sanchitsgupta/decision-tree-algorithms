#ifndef ATTRIBUTES
#define ATTRIBUTES

#include "attribute_maps.h"
#include "attributes_structure.h"

//define all attributes
Attribute age(0, 2), workclass(1, 8, workclass_map), fnlwgt(2, 2), education(3, 16, education_map);
Attribute education_num(4, 2), marital_status(5, 7, marital_map), occupation(6, 14, occupation_map);
Attribute relationship(7, 6, relationship_map), race(8, 5, race_map), sex(9, 2, sex_map), capital_gain(10, 2);
Attribute capital_loss(11, 2), hours_per_week(12, 2), native_country(13, 41, country_map);

#endif
