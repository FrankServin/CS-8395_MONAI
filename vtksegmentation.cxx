#include <vtkXMLPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include <vtkLookupTable.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMarchingCubes.h>
#include <vtkRendererCollection.h>
#include <vtkIdList.h>
#include <vtkCommand.h>

#include <iostream>



//creates an interactive picker using vtkinteractorStyletrackballCamera
class CustomInteractor : public vtkInteractorStyleTrackballCamera
{
public:
    Customnteractor()
    {
    }

    ~CustomPickingInteractor()
    {
    }

    static CustomInteractor* New()
    {
        CustomInteractor* interactor = new CustomInteractor();
        return interactor;
    }

    virtual void OnLeftButtonDown() override
    {

        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
};

int main(int argc, char* argv[])
{
    // argv[1] is going to be the file name for the first polydata
    // argv[2] is going to be the file name for the second polydata
    if (argc < 3)
    {
        std::cout << "you need to have an argument" << std::endl;
        return -1;
    }

    //////////////////////////////////read the segmentation
    /////////////////////////////////

    // read the segmentation data
    vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
    polydataReader->SetFileName(argv[1]);
    polydataReader->Update();

    vtkSmartPointer < vtkPolyData > polyData = polydataReader->GetOutput();

    // Create a 3D model using marching cubes
    vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
    mc->SetInputConnection(reader->GetOutputPort());
    mc->ComputeNormalsOn();
    mc->ComputeGradientsOn();
    mc->SetValue(0, threshold);  // second value acts as a default threshold

    // set up the mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(mc->GetOutputPort());

    mapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);

    //////////////////////////////////read the original image file
    ////////////////////////////////////

    // read the data file in
    vtkSmartPointer < vtkXMLPolyDataReader > polydataReader2 = vtkSmartPointer < vtkXMLPolyDataReader >::New();
    polydataReader2->SetFileName(argv[2]);
    polydataReader2->Update();
    vtkSmartPointer < vtkPolyData > polyData2 = polydataReader2->GetOutput();

   
    // set up the first mapper too
    mapper->SetInputData(polyData);
    mapper->ScalarVisibilityOn();

    // set up the second  mapper
    vtkSmartPointer < vtkPolyDataMapper > mapper2 = vtkSmartPointer < vtkPolyDataMapper >::New();
    mapper2->SetInputData(polyData2);

    // set up the actor
    vtkSmartPointer < vtkActor > actor2 = vtkSmartPointer < vtkActor > ::New();
    actor2->SetMapper(mapper2);
    //set the color to default/grayscale
    actor2->GetProperty()->SetColor(0.0, 0.0, 0.0);

    // now put it all together
    ///////////////////////////////

    // set up the scene with all of the objects in the scene (just one for now)
    vtkSmartPointer < vtkRenderer > renderer = vtkSmartPointer < vtkRenderer > ::New();
    renderer->AddActor(actor);
    renderer->AddActor(actor2);

    // create a window
    vtkSmartPointer < vtkRenderWindow > window = vtkSmartPointer < vtkRenderWindow >::New();
    window->AddRenderer(renderer);

    // create the interactor
    vtkSmartPointer < vtkRenderWindowInteractor > interactor = vtkSmartPointer < vtkRenderWindowInteractor > ::New();
    interactor->SetRenderWindow(window);


    //the custom interactor is set using the trackball camera
    vtkSmartPointer < CustomInteractor > myStyle = vtkSmartPointer < CustomInteractor >::New();
    interactor->SetInteractorStyle(myStyle);

    // before i start, i will create a timer, and a callback for any timer events
    vtkSmartPointer < vtkCustomTimerCallback > myCallback = vtkSmartPointer < vtkCustomTimerCallback>::New();
    myCallback->SetActor(actor, 5);

    //initialize the mapper as a 
    interactor->Initialize();


    // now start the interaction as usual
    interactor->Start();

    return 0;

}