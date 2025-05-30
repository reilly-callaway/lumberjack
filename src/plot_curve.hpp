#ifndef PLOT_CURVE_H
#define PLOT_CURVE_H

#include <QThread>

#include <qwt_plot_curve.h>

#include "data_series.hpp"
#include "plot_sampler.hpp"

class PlotCurve;

/*
 * An extension of the QwtPlotCurve class,
 * which links directly to a TimeSeries object.
 *
 * Includes intelligent down-sampling functionality,
 * so that curve features are retained at low zoom levels.
 *
 * A background thread performs the task of re-sampling data (as required).
 *
 */
class PlotCurve : public QObject, public QwtPlotCurve
{
   Q_OBJECT

public:
    PlotCurve(DataSeriesPointer series, PlotCurveUpdater* updater);
    PlotCurve(DataSeries* series, PlotCurveUpdater *updater) : PlotCurve(DataSeriesPointer(series), updater) {}

    DataSeriesPointer getDataSeries(void) { return series; }

    virtual ~PlotCurve();

public slots:
    void resampleData(double t_min, double t_max, unsigned int n_pixels);
    void updateLabel(void);
    void updateLineStyle(void);

    virtual void setVisible(bool on) override;

protected slots:
    void onDataResampled(const QVector<double> &t_data, const QVector<double> &y_data);

protected:
    DataSeriesPointer series;

    PlotCurveUpdater *worker = nullptr;

    //! Thread for re-sampling curve data
    QThread workerThread;
};

#endif // PLOT_CURVE_H
