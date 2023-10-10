#ifndef vtkHyperPoints_h
#define vtkHyperPoints_h

#include "vtkArrayDispatch.h"
#include "vtkDataArray.h"
#include "vtkDataArrayRange.h"
#include "vtkFloatArray.h"
#include "vtkIdList.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"

class vtkDataArray2 : public vtkFloatArray {
public:
    static vtkDataArray2 *New();
    vtkTypeMacro(vtkDataArray2, vtkDataArray);

    bool ComputeScalarRange(double *range) override {
        return this->Superclass::ComputeScalarRange(range);
    };
};

class VTK_EXPORT vtkHyperPoints : public vtkPoints {
    vtkTypeMacro(vtkHyperPoints, vtkPoints);

public:
    static vtkHyperPoints *New();

    void PrintSelf(ostream &os, vtkIndent indent) override;

    vtkIdType GetNumberOfPoints() const {
        return this->Data->GetNumberOfTuples();
    }
    void GetPoints(vtkIdList *ptIds, vtkHyperPoints *outPoints);

    double *GetPoint(vtkIdType id)
        VTK_EXPECTS(0 <= id && id < GetNumberOfPoints()) VTK_SIZEHINT(4) {
        return this->Data->GetTuple(id);
    }

    void GetPoint(vtkIdType id, double x[4])
        VTK_EXPECTS(0 <= id && id < GetNumberOfPoints()) VTK_SIZEHINT(4) {
        this->Data->GetTuple(id, x);
    }

    void SetPoint(vtkIdType id, const float x[4])
        VTK_EXPECTS(0 <= id && id < GetNumberOfPoints()) {
        this->Data->SetTuple(id, x);
    }
    void SetPoint(vtkIdType id, const double x[4])
        VTK_EXPECTS(0 <= id && id < GetNumberOfPoints()) {
        this->Data->SetTuple(id, x);
    }
    void SetPoint(vtkIdType id, double x, double y, double z, double w)
        VTK_EXPECTS(0 <= id && id < GetNumberOfPoints());

    void InsertPoint(vtkIdType id, const float x[4]) VTK_EXPECTS(0 <= id) {
        this->Data->InsertTuple(id, x);
    }
    void InsertPoint(vtkIdType id, const double x[4]) VTK_EXPECTS(0 <= id) {
        this->Data->InsertTuple(id, x);
    }
    void InsertPoint(vtkIdType id, double x, double y, double z, double w)
        VTK_EXPECTS(0 <= id);

    void SetNumberOfPoints(vtkIdType numPoints);

    vtkTypeBool Resize(vtkIdType numPoints);

    vtkIdType InsertNextPoint(const float x[4]) {
        return this->Data->InsertNextTuple(x);
    }
    vtkIdType InsertNextPoint(const double x[4]) {
        return this->Data->InsertNextTuple(x);
    }
    vtkIdType InsertNextPoint(double x, double y, double z, double w);

    void ComputeBounds() override;
    double *GetBounds() VTK_SIZEHINT(8);
    void GetBounds(double bounds[8]);
    double Bounds[8];

    vtkGetObjectMacro(Data, vtkDataArray2);

protected:
    vtkHyperPoints();
    ~vtkHyperPoints() override;
    vtkDataArray2 *Data;

private:
    vtkHyperPoints(const vtkHyperPoints &) = delete;
    void operator=(const vtkHyperPoints &) = delete;
};

inline void vtkHyperPoints::SetNumberOfPoints(vtkIdType numPoints) {
    this->Data->SetNumberOfComponents(4);
    this->Data->SetNumberOfTuples(numPoints);
    this->Modified();
}

inline vtkTypeBool vtkHyperPoints::Resize(vtkIdType numPoints) {
    this->Data->SetNumberOfComponents(4);
    this->Modified();
    return this->Data->Resize(numPoints);
}

inline void vtkHyperPoints::SetPoint(vtkIdType id, double x, double y, double z,
                                     double w) {
    double p[4] = {x, y, z, w};
    this->Data->SetTuple(id, p);
}

inline void vtkHyperPoints::InsertPoint(vtkIdType id, double x, double y,
                                        double z, double w) {
    double p[4] = {x, y, z, w};
    this->Data->InsertTuple(id, p);
}

inline vtkIdType vtkHyperPoints::InsertNextPoint(double x, double y, double z,
                                                 double w) {
    double p[4] = {x, y, z, w};
    return this->Data->InsertNextTuple(p);
}
#endif
