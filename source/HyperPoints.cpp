#include "HyperPoints.hpp"

VTK_ABI_NAMESPACE_BEGIN

vtkStandardNewMacro(vtkDataArray2);
vtkHyperPoints *vtkHyperPoints::New() {
    // First try to create the object from the vtkObjectFactory
    vtkObject *ret = vtkObjectFactory::CreateInstance("vtkHyperPoints");
    if (ret) {
        return static_cast<vtkHyperPoints *>(ret);
    }
    // If the factory was unable to create the object, then create it here.
    vtkHyperPoints *result = new vtkHyperPoints();
    result->InitializeObjectBase();
    return result;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
struct GetTuplesFromListWorker {
    vtkIdList *Ids;

    GetTuplesFromListWorker(vtkIdList *ids)
        : Ids(ids) {
    }

    template <typename Array1T, typename Array2T>
    void operator()(Array1T *src, Array2T *dst) const {
        const auto srcTuples = vtk::DataArrayTupleRange<4>(src);
        auto dstTuples = vtk::DataArrayTupleRange<4>(dst);

        vtkIdType *srcTupleId = this->Ids->GetPointer(0);
        vtkIdType *srcTupleIdEnd =
            this->Ids->GetPointer(this->Ids->GetNumberOfIds());

        auto dstTupleIter = dstTuples.begin();
        while (srcTupleId != srcTupleIdEnd) {
            *dstTupleIter++ = srcTuples[*srcTupleId++];
        }
    }
};

//------------------------------------------------------------------------------
// Given a list of pt ids, return an array of points.
void vtkHyperPoints::GetPoints(vtkIdList *ptIds, vtkHyperPoints *outPoints) {
    outPoints->Data->SetNumberOfTuples(ptIds->GetNumberOfIds());

    // We will NOT use this->Data->GetTuples() for 4 reasons:
    // 1) It does a check if the outPoints->Data array is a vtkDataArray, which we now it is.
    // 2) It does a check if the number of components is the same, which we know it's 3 for both.
    // 3) It has a really expensive Dispatch2::Execute, and every time it tries many array types.
    //    Points are 99% of the times floats or doubles, so we can avoid A LOT of failed FastDownCast
    //    operations, by utilizing this knowledge.
    // 4) The Worker isn't aware of the number of components of the tuple which slows down the access.
    using Dispatcher =
        vtkArrayDispatch::Dispatch2ByValueType<vtkArrayDispatch::Reals,
                                               vtkArrayDispatch::Reals>;
    GetTuplesFromListWorker worker(ptIds);
    if (!Dispatcher::Execute(this->Data, outPoints->Data, worker)) {
        // Use fallback if dispatch fails.
        worker(this->Data, outPoints->Data);
    }
}

vtkHyperPoints::vtkHyperPoints() {
    this->Data = vtkDataArray2::New();
    this->Data->Register(this);
    this->Data->Delete();

    this->Data->SetNumberOfComponents(4);
    this->Data->SetName("Points");
    this->Bounds[0] = this->Bounds[2] = this->Bounds[4] = this->Bounds[6] =
        VTK_DOUBLE_MAX;
    this->Bounds[1] = this->Bounds[3] = this->Bounds[5] = this->Bounds[7] =
        -VTK_DOUBLE_MAX;
}

vtkHyperPoints::~vtkHyperPoints() {
}

void vtkHyperPoints::ComputeBounds() {
    if (this->GetMTime() > this->ComputeTime) {
        this->Data->ComputeScalarRange(this->Bounds);
        this->ComputeTime.Modified();
    }
}
void vtkHyperPoints::GetBounds(double bounds[8]) {
    this->ComputeBounds();
    memcpy(bounds, this->Bounds, 8 * sizeof(double));
}

double *vtkHyperPoints::GetBounds() {
    this->ComputeBounds();
    return this->Bounds;
}
VTK_ABI_NAMESPACE_BEGIN
void vtkHyperPoints::PrintSelf(ostream &os, vtkIndent indent) {
    this->vtkObject::PrintSelf(os, indent);

    os << indent << "Data: " << this->Data << "\n";
    os << indent << "Data Array Name: ";
    if (this->Data->GetName()) {
        os << this->Data->GetName() << "\n";
    } else {
        os << "(none)\n";
    }

    os << indent << "Number Of Points: " << this->GetNumberOfPoints() << "\n";
    const double *bounds = this->GetBounds();
    os << indent << "Bounds: \n";
    os << indent << "  Xmin,Xmax: (" << bounds[0] << ", " << bounds[1] << ")\n";
    os << indent << "  Ymin,Ymax: (" << bounds[2] << ", " << bounds[3] << ")\n";
    os << indent << "  Zmin,Zmax: (" << bounds[4] << ", " << bounds[5] << ")\n";
    os << indent << "  Wmin,Wmax: (" << bounds[6] << ", " << bounds[7] << ")\n";
}
VTK_ABI_NAMESPACE_END
