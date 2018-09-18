// Copyright 2013 Velodyne Acoustics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVelodyneHDLReader.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkVelodyneHDLReader - class for reading Velodyne HDL data
// .Section Description
//

#ifndef _vtkVelodyneHDLReader_h
#define _vtkVelodyneHDLReader_h

#include "vtkLidarReader.h"
#include "vtkDataPacket.h"
#include <string>
#include <vtkSmartPointer.h>

class vtkVelodyneTransformInterpolator;

using DataPacketFixedLength::HDL_MAX_NUM_LASERS;

class VTK_EXPORT vtkVelodyneHDLReader : public vtkLidarReader
{
public:
  enum DualFlag
  {
    DUAL_DISTANCE_NEAR = 0x1,  // point with lesser distance
    DUAL_DISTANCE_FAR = 0x2,   // point with greater distance
    DUAL_INTENSITY_HIGH = 0x4, // point with lesser intensity
    DUAL_INTENSITY_LOW = 0x8,  // point with greater intensity
    DUAL_DOUBLED = 0xf,        // point is single return
    DUAL_DISTANCE_MASK = 0x3,
    DUAL_INTENSITY_MASK = 0xc,
  };

public:
  static vtkVelodyneHDLReader* New();
  vtkTypeMacro(vtkVelodyneHDLReader, vtkLidarReader);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  //
  bool IsIntensityCorrectedBySensor();
  const bool& GetWantIntensityCorrection();
  void SetIntensitiesCorrected(const bool& state);

  void GetLaserCorrections(double verticalCorrection[HDL_MAX_NUM_LASERS],
    double rotationalCorrection[HDL_MAX_NUM_LASERS], double distanceCorrection[HDL_MAX_NUM_LASERS],
    double distanceCorrectionX[HDL_MAX_NUM_LASERS], double distanceCorrectionY[HDL_MAX_NUM_LASERS],
    double verticalOffsetCorrection[HDL_MAX_NUM_LASERS],
    double horizontalOffsetCorrection[HDL_MAX_NUM_LASERS], double focalDistance[HDL_MAX_NUM_LASERS],
    double focalSlope[HDL_MAX_NUM_LASERS], double minIntensity[HDL_MAX_NUM_LASERS],
    double maxIntensity[HDL_MAX_NUM_LASERS]);

  void GetXMLColorTable(double XMLColorTable[]);

  int GetOutputPacketProcessingDebugInfo() const;
  void SetOutputPacketProcessingDebugInfo(int);

  int GetIntraFiringAdjust() const;
  void SetIntraFiringAdjust(int);

  unsigned int GetDualReturnFilter() const;
  void SetDualReturnFilter(unsigned int);

  void SetFiringsSkip(int);

  // I/O and processing functions
  int ReadFrameInformation();
  int GetNumberOfFrames() override;

  void DumpFrames(int startFrame, int endFrame, const std::string& filename);

  std::vector<vtkSmartPointer<vtkPolyData> >& GetDatasets();

  void appendRollingDataAndTryCorrection(const unsigned char* data);

  bool getIsHDL64Data();

  bool isReportedSensorAndCalibrationFileConsistent(bool shouldWarn);
  bool updateReportedSensor(const unsigned char* data, unsigned int bytesReceived);

  bool GetHasDualReturn();

  // This function permits to know which are the points selected
  // with a corresponding dual return
  void SetSelectedPointsWithDualReturn(double* data, int Npoints);
  void SetShouldAddDualReturnArray(bool input);

  // Information about the sensor from dataPacket
//  std::string GetSensorInformation() override;

protected:
  class vtkInternal;
  vtkVelodyneHDLReader();
  vtkVelodyneHDLReader(vtkInternal* pimpl);
  ~vtkVelodyneHDLReader();

  int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*);

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*);

  void SetTimestepInformation(vtkInformation* info);

private:
  vtkInternal* Internal;
  vtkVelodyneHDLReader(const vtkVelodyneHDLReader&) = delete;
  void operator=(const vtkVelodyneHDLReader&) = delete;
};
#endif
