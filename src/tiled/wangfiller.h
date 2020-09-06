/*
 * wangfiller.h
 * Copyright 2017, Benjamin Trotter <bdtrotte@ucsc.edu>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "grid.h"
#include "map.h"
#include "wangset.h"

#include <QList>
#include <QMap>
#include <QPoint>

#include <memory>

namespace Tiled {

class StaggeredRenderer;

/**
 * WangFiller provides functions for choosing cells based on a surrounding map
 * and a wangSet.
 *
 * Optionally when choosing cells, this will look at adjacent cells
 * to ensure that they will be able to be filled based on the chosen cell.
 */
class WangFiller
{
public:
    struct CellInfo {
        WangId desired;
        WangId mask;

        bool operator==(const CellInfo &other) const {
            return desired == other.desired && mask == other.mask;
        }
    };

    explicit WangFiller(const WangSet &wangSet,
                        const StaggeredRenderer *staggeredRenderer = nullptr);

    /**
     * Finds a cell from the attached WangSet which fits the given
     * surroundings.
     */
    Cell findFittingCell(const TileLayer &back,
                         const TileLayer &front,
                         const QRegion &region,
                         QPoint point) const;

    /**
     * Fills the given \a region in the \a target layer with Wang methods.
     */
    void fillRegion(TileLayer &target, const TileLayer &back, const QRegion &region) const;

    /**
     * Fills the given \a region in the \a target layer with Wang methods,
     * based on the desired \a wangIds.
     */
    void fillRegion(TileLayer &target, const TileLayer &back, Grid<CellInfo> wangIds, const QRegion &region) const;

private:
    /**
     * Returns a cell from either the \a back or \a front, based on the
     * \a region. \a point, \a front, and \a region are relative to \a back.
     */
    const Cell &getCell(const TileLayer &back,
                        const TileLayer &front,
                        const QRegion &region,
                        QPoint point) const;

    /**
     * Returns a wangId based on \a front and \a back. Adjacent cells are
     * obtained using getCell().
     */
    WangId wangIdFromSurroundings(const TileLayer &back,
                                  const TileLayer &front,
                                  const QRegion &region,
                                  QPoint point) const;

    /**
     * Returns a wangId based on cells from \a back which are not in the
     * \a region. \a point and \a region are relative to \a back.
     */
    WangId wangIdFromSurroundings(const TileLayer &back,
                                  const QRegion &region,
                                  QPoint point) const;

    const WangSet &mWangSet;
    const StaggeredRenderer * const mStaggeredRenderer;
};

} // namespace Tiled
