#include <QFileDialog>
#include <QGridLayout>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "DataWriter.h"
#include <QDebug>
#include "Transformer.h"
using namespace Geometry;
using namespace Transformation;

Visualizer::Visualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();

    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
    connect(translate, &QPushButton::clicked, this, &Visualizer::onTranslateClick);
    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
}

Visualizer::~Visualizer()
{

}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    translate = new QPushButton("Translate", this);
    exportFile= new QPushButton("Export", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidget(this);

    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0, 1, 2);
    layout->addWidget(translate, 0, 2, 1, 2);
    layout->addWidget(exportFile, 0, 4, 1, 2);
    layout->addWidget(openglWidgetInput, 1, 0, 1, 3);
    layout->addWidget(openglWidgetOutput, 1, 3, 1, 3);
    
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
}

void Visualizer::readFile(const QString& inFileName)
{
    if (inFileName.endsWith(".stl", Qt::CaseInsensitive))
    {
        STLReader reader;
        reader.read(inFileName.toStdString(), inTriangulation);
    }
    else if (inFileName.endsWith(".obj", Qt::CaseInsensitive))
    {
        OBJReader reader;
        reader.read(inFileName.toStdString(), inTriangulation);
    }
}

void Visualizer::writeFile()
{
    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.obj)"));
        qInfo() << fileName << endl;
        ObjWriter writer;
        writer.Write(fileName.toStdString(), outTriangulation);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.stl)"));
        STLWriter writer;
        writer.Write(fileName.toStdString(), outTriangulation);
    }
}

void  Visualizer::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        readFile(inputFilePath);
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(inTriangulation);
        openglWidgetInput->setData(data);
    }
    else
    {
        msgBox.setText("No file was selected");
        msgBox.exec();
    }
}

void Visualizer::onTranslateClick()
{
    if (!inputFilePath.isEmpty())
    {
        QFileDialog dialog(this);

        dialog.setFileMode(QFileDialog::Directory);

        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
            "/home",
            QFileDialog::ShowDirsOnly
            | QFileDialog::DontResolveSymlinks);

        // Check if directory is selected
        if (!dir.isEmpty())
        {
            QString exportFileName;

            if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
            {
                exportFileName = QDir(dir).filePath("temp.obj");
                
                Transformation::Transformer transformer;
                outTriangulation = transformer.scaleUniform(inTriangulation, 2.0);

                ObjWriter writer;
                writer.Write(exportFileName.toStdString(), outTriangulation);

                /*qInfo() << outTriangulation.uniqueNumbers.size();
                qInfo() << inTriangulation.uniqueNumbers.size();*/

                /*for (int i = 0; i < inTriangulation.uniqueNumbers.size(); i++)
                    qInfo() << inTriangulation.uniqueNumbers[i] << endl;
                qInfo() << "Now next" << endl;
                for (int i = 0; i < outTriangulation.uniqueNumbers.size(); i++)
                    qInfo() << outTriangulation.uniqueNumbers[i] << endl;*/

                // reload file to check and load in output renderer
                OBJReader reader;
                reader.read(exportFileName.toStdString(), outTriangulation);

                QFile::remove(exportFileName);
            }

            else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
            {
                exportFileName = QDir(dir).filePath("temp.stl");
                STLWriter writer;
                writer.Write(exportFileName.toStdString(), inTriangulation);

                // reload file to check and load in output renderer
                STLReader reader;
                reader.read(exportFileName.toStdString(), outTriangulation);

                QFile::remove(exportFileName);
            }

            OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outTriangulation);
            openglWidgetOutput->setData(data);
        }
        else
        {
            msgBox.setText("No directory was selected!");
            msgBox.exec();
        }
    }
    else
    {
        msgBox.setText("Input file was not loaded!");
        msgBox.exec();
    }
}

void Visualizer::onExportClick()
{
    if (!inputFilePath.isEmpty())
    {
        QFileDialog dialog(this);
        writeFile();
    }
    else
    {
        msgBox.setText("Input file was not loaded!");
        msgBox.exec();
    }
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& triangulation)
{
    OpenGlWidget::Data data;
    for  (Triangle triangle : triangulation.Triangles)
    {
        for each (Point point in triangle.Points())
        {
            data.vertices.push_back(triangulation.uniqueNumbers[point.X()]);
            data.vertices.push_back(triangulation.uniqueNumbers[point.Y()]);
            data.vertices.push_back(triangulation.uniqueNumbers[point.Z()]);
        }

        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(triangulation.uniqueNumbers[normal.X()]);
            data.normals.push_back(triangulation.uniqueNumbers[normal.Y()]);
            data.normals.push_back(triangulation.uniqueNumbers[normal.Z()]);
        }
    }
    return data;
}
