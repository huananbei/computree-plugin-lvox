#include <QtTest>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFile>

#include <memory>
#include <cmath>
#include <iostream>

#include "Eigen/Dense"
#include "Eigen/Geometry"

#include "ct_global/ct_context.h"
#include "ct_defines.h"

#include "ct_itemdrawable/tools/scanner/ct_thetaphishootingpattern.h"
#include "ct_itemdrawable/tools/scanner/ct_shootingpatternfrompointcloud.h"
#include "ct_itemdrawable/tools/scanner/ct_parallelshootingpatternfrompointcloud.h"
#include "ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.h"
#include "mk/tools/worker/lvox3_computetheoriticals.h"
#include "mk/tools/lvox3_gridtype.h"
#include "mk/tools/lvox3_errorcode.h"
#include "mk/tools/worker/lvox3_computeall.h"
#include "mk/tools/traversal/woo/lvox3_grid3dwootraversalalgorithm.h"

using namespace Eigen;
using namespace std;

/*
 * SRCDIR is defined by the compiler, but Eclipse doesn't
 * know it and complains. Define a value here to make it happy.
 */
#ifndef SRCDIR
#define SRCDIR "."
#endif

QDebug operator<<(QDebug debug, const Vector3d &v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')';

    return debug;
}

class ScannersTest : public QObject
{
    Q_OBJECT

public:
    ScannersTest();

private Q_SLOTS:
    void testTLS();
    void testPointCloudShootingPattern();
    void testParallelShootingPattern();
    void testBoudingBox();
    void testTheoreticalGrid();

private:
    CT_PCIR m_pcir;
};

ScannersTest::ScannersTest()
{
    CT_AbstractUndefinedSizePointCloud* cloud =
            PS_REPOSITORY->createNewUndefinedSizePointCloud();
    cloud->addPoint(CT_Point(Vector3d(1, 2, 3)));
    cloud->addPoint(CT_Point(Vector3d(4, 5, 6)));
    m_pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(cloud);
}

void ScannersTest::testTLS()
{
    Vector3d origin(0, 0, 0);
    Vector3d normal(0, 0, 1);
    double theta = 90.;
    double phi = 90.;
    double res = 1.;
    size_t n = 8100;

    /*
     * Create a stub shooting pattern with 90 degrees of field of view with 1
     * degree of resolution.
     */

    CT_ThetaPhiShootingPattern p(origin, theta, phi, res, res, 0, 0, normal,
                                 false, false);
    QVERIFY2(p.getNumberOfShots() == n, "wrong number of shots");

    for (uint i = 0; i < n - 1; i++) {
        CT_Shot s1 = p.getShotAt(i);
        CT_Shot s2 = p.getShotAt(i + 1);
        QVERIFY2(s1.getOrigin() == origin, "s1 origin must match");
        QVERIFY2(s2.getOrigin() == origin, "s2 origin must match");

        Vector3d cross = s1.getDirection().cross(s2.getDirection());
        QVERIFY2(cross.norm() > 0., "shots are not expected to be parallel");
    }
}


void ScannersTest::testPointCloudShootingPattern()
{
    /*
     * Create a cloud of points for CT_ShootingPatternFromPointCloud
     */

    Vector3d origin(10, 10, 10);
    CT_ShootingPatternFromPointCloud p(origin, m_pcir);
    QVERIFY(p.getNumberOfShots() == 2);
    CT_Shot s1 = p.getShotAt(0);
    CT_Shot s2 = p.getShotAt(1);
    Vector3d cross = s1.getDirection().cross(s2.getDirection());
    QVERIFY2(cross.norm() > 0, "shots are not expected to be parallel");
}

void ScannersTest::testParallelShootingPattern()
{
    /*
     * The plane is above the points and rays are downwards (nadir)
     */
    Vector3d origin(0, 0, 10);
    Vector3d direction(0, 0, -1);

    CT_ParallelShootingPatternFromPointCloud p(origin, direction, m_pcir);

    QVERIFY(p.getNumberOfShots() == 2);
    CT_Shot s1 = p.getShotAt(0);
    CT_Shot s2 = p.getShotAt(1);
    Vector3d cross1 = s1.getDirection().cross(direction);
    Vector3d cross2 = s2.getDirection().cross(direction);
    QCOMPARE(cross1.norm(), 0.);
    QCOMPARE(cross2.norm(), 0.);
}

void ScannersTest::testBoudingBox()
{
    AlignedBox3d b1(Vector3d(-1, -2, -3), Vector3d(1, 2, 3));
    AlignedBox3d b2(Vector3d(-3, -2, -1), Vector3d(3, 2, 3));
    b1.extend(b2);
    Vector3d sizes = b1.sizes();
    QVERIFY(sizes == Vector3d(6, 4, 6));
    QVERIFY(b1.center() == Vector3d(0, 0, 0));

    /*
     * Top/Bottom really means Front/Back
     */
    QVERIFY(b1.corner(AlignedBox3d::BottomLeftFloor) == Vector3d(-3, -2, -3));
    QVERIFY(b1.corner(AlignedBox3d::BottomRightFloor) == Vector3d(3, -2, -3));
    QVERIFY(b1.corner(AlignedBox3d::TopLeftFloor) == Vector3d(-3, 2, -3));
    QVERIFY(b1.corner(AlignedBox3d::TopRightFloor) == Vector3d(3, 2, -3));

    QVERIFY(b1.corner(AlignedBox3d::BottomLeftCeil) == Vector3d(-3, -2, 3));
    QVERIFY(b1.corner(AlignedBox3d::BottomRightCeil) == Vector3d(3, -2, 3));
    QVERIFY(b1.corner(AlignedBox3d::TopLeftCeil) == Vector3d(-3, 2, 3));
    QVERIFY(b1.corner(AlignedBox3d::TopRightCeil) == Vector3d(3, 2, 3));
}

class DebugVisitor : public LVOX3_Grid3DVoxelWooVisitor
{
public:
    DebugVisitor() {}
    void visit(const LVOX3_Grid3DVoxelWooVisitorContext& context) {
        qDebug() << "visit: " << context.colLinLevel.x() << context.colLinLevel.y() << context.colLinLevel.z();
    }
};

void ScannersTest::testTheoreticalGrid()
{
    /*
        const Eigen::Vector3d& origin,
                               double hFov, double vFov,
                               double hRes, double vRes,
                               double initTheta, double initPhi,
                               const Eigen::Vector3d& zVector = Eigen::Vector3d(0,0,1),
                               bool clockWise = true,
                               bool radians = false
    */

    double z = 10;
    CT_ThetaPhiShootingPattern pattern(
        Vector3d(5., 1., z),
        90, 180,
        10, 10,
        0., 0.);

    lvox::Grid3Di grid(NULL, NULL,
            10., 10., 10.,
            10UL, 10UL, 10UL,
            1, lvox::Max_Error_Code, 0);
    qDebug() << "grid.nCells()" << grid.nCells();

    Vector3d min, max;
    grid.getBoundingBox(min, max);
    cout << min << "\n" << max << endl;

    QVector<LVOX3_Grid3DVoxelWooVisitor*> visitors;
    visitors.append(new DebugVisitor());
    LVOX3_Grid3DWooTraversalAlgorithm<lvox::Grid3DiType> algo(&grid, true, visitors);

    size_t n = pattern.getNumberOfShots();
    qDebug() << "number of shots" << n;
    for (size_t i = 0; i < n; i++) {
        const CT_Shot& shot = pattern.getShotAt(i);
        algo.compute(shot.getOrigin(), shot.getDirection());
    }

    /*
     * It is mandatory to allocate the object here with "new", the
     * workersManager calls "delete" on all workers in its destructor.
     */
//    LVOX3_ComputeTheoriticals *worker = new LVOX3_ComputeTheoriticals(&pattern, &grid);
//    LVOX3_ComputeAll workersManager;
//    workersManager.addWorker(0, worker);
//    workersManager.compute();

}

QTEST_APPLESS_MAIN(ScannersTest)

#include "tst_scanners.moc"
