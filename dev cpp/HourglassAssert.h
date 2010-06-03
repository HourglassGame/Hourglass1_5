/*
 *  AllegroAssert.h
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 1/06/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#ifndef ALLEGRO_ASSERT
#define ALLEGRO_ASSERT
#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>
#define hg_assert(x) BOOST_ASSERT(x)
#endif //ALLEGRO_ASSERT
