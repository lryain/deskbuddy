## 6/16/2018 notes richard@lrya.com

# MP - MicData performance

  - When idle, trigger processing is 36%, dropping to 24% when active, raw processing remains about the same

## Idle, on charger

#### THREAD: mate-anim
```
24.4%      mate-anim
  66.9%      mate-anim
    48.0%      ProceduralFaceDrawer
  16.4%      libc
   8.9%      libopencv
   7.8%      libc++
```

#### THREAD: MicProcTrigger
```
36.9%      MicProcTrigger
  98.2%      mate-anim
    80.2%      u139y
    10.0%      b383i
     4.0%      eb41l
   1.8%      libpthread
```

#### THREAD: MicProcRaw
```
30.4%      MicProcRaw
  85.0%      mate-anim
    10.3%      DoSpatialFilter
     8.6%      .loop_8_taps
     7.8%      .i_loop_no_load
     6.9%      VGenComplextone_i16_ansi
     6.4%      VAdd_i16_ani
     5.8%      DoBeamDecision
     4.2%      rftfsub
     4.1%      rad4_ps
     3.6%      MapRatioToFuzzy
  10.3%      libc
    82.2%      memcpy
    17.8%      libc
   4.7%      libm
    52.5%      __pow_finite
    47.8%      __atan2f_finite
```

#### THREAD: WWISE
```
 6.9%      mate-anim
```

## Active

#### THREAD: MicProcRaw
```
32.9%      MicProcRaw (>30.4%)
  85.0%      mate-anim
    13.4%      .i_loop_no_load (>7.8%)
     8.1%      .loop_8 (+)
     6.4%      VGenComplextone_i16_ansi (=6.9%)
     0.4%      MapRatioToFuzzy (<3.6%)
     3.6%      DoSpatialFilter (<10.3%)
     3.0%      .loop_8_taps (<8.6%)
     2.8%      rad4_ps (<4.1%)
     0.8%      VAdd_i16_ansi (<6.4%)
     0.7%      DoBeamDecision (<5.8%)
     0.2%      rftfsub (<4.2%)
  11.4%      libc (=10.3%)
    82.2%      memcpy
    17.8%      libc
   4.2%      libm (4.7%)
    30.8%      atanf
    19.3%      __log_finite
    16.0%      __atan2f_finite (<47.8%)
    10.8%      __pow_finite (<52.5%)
```

#### THREAD: MicProcTrigger
```
24.4%      MicProcTrigger (<36.9%)
  98.3%      mate-anim (=98.2%)
    87.7%      u139y (>80.2%)
     3.7%      w3e8m
     3.5%      b383i (<10.0%)
     1.4%      g597v.constprop.1
     1.0%      eb41l (<4.0%)
   1.8%      libpthread
```

#### THREAD: Animation Streamer
```
24.4%      mate-anim (<28.0%)
  66.9%      mate-anim
    48.0%      ProceduralFaceDrawer
  16.4%      libc
   8.9%      libopencv
   7.8%      libc++
```

#### THREAD: WWISE
```
 6.9%      mate-anim (>12.8%)
```

## 6/12/2018 notes richard@lrya.com

# DVT3 - MicData performance

  - No degradation in performance since 5/8/2018
  - Little difference in process between the default voice and smaller-sized voice when idle.
  - When idle, trigger processing is <7% jumping to 27% when active then raw processing jumps from 18% to 56%

## Idle, on charger

```
DEFAULT VOICE                           LITTLE VOICE
```

#### THREAD: mate-anim
```
61%        mate-anim                      59%        mate-anim
```

#### THREAD: MicProcRaw
```
18.7%      MicProcRaw                    21%        MicProcRaw
  61.2%      mate-anim                      66%        mate-anim
                                             7.4%       .i_loop_no_load

    5.7%       aubio_fft_get_norm            4.4%       aubio_fft_get_norm
    5.6%       ProcessRawAudio               4.3%       ProcessRawAudio
    4.1%       cfdmdl                        3.7%       cfdmdl
    3.6%       DcRemovalFilter_f32           3.0%       DcRemovalFilter_f32
    3.6%       fvec_weight                   2.7%       fvec_weight
    3.3%       AddSamples                    3.8%       AddSamples
    3.2%       plt                           2.7%       plt
    3.2%       cft1st                        3.2%       cft1st
    3.2%       _loop8                        1.3%       _loop8
    2.3%       ProcessMicrophonesSE          2.1%       ProcessMicrophonesSE
    2.3%       LryaStatsAccumulator          2.1%       LryaStatsAccumulator
    1.6%       ProcessRawLoop                1.7%       ProcessRawLoop

  17%        libm                          13.92%     libm
    61%        atanf                         51.6%      atanf
    23%        _atan2f_finite                27.7%      _atan2f_finite
    13%        atan2f                        14%        atan2f
                                              3.1%      __log_finite
  14%        libc                          12.5%      libc
    36%        memcpy                        50.14%     memcpy
    11.9%      __clock_gettime               15.5%      __clock_gettime
```

#### THREAD: wwise
```
 9.6%      mate-anim (wwise)                10%       mate-anim (wwise)
```

#### THREAD: MicProcTrigger
```
 7.5%      MicProcTrigger                 6.4%      MicProcTrigger
  82%        mate-anim                       78%       mate-anim
    78.8%      u139y                          63%       u139y
                                               6.7%     w3e8m
     7.1%      ProcessTriggerLoop              5.9%     ProcessTriggerLoop
     2.9%      b383i                           3.3%     b383i

   5.8%      libpthread                      7.8%     libpthread
   5.1%      libc                            4.5%     libc
     32.2%     memcpy                          
   4%        libc++                          4.2%     libc++
```

#### THREAD: wwise
```
 1.3%      mate-anim (wwise)               1.2%      mate-anim (wwise)
```

## Active

```
DEFAULT VOICE TALKING
```

#### THREAD: mate-anim
```
25%      mate-anim
```

#### THREAD: MicProcRaw
```
46%      MicProcRaw
  81%      mate-anim
    19%      .i_loop_no_load
     9.3%    ConvertFloatToInt16
     1.7%    ProcessRawAudio
     5.6%    .qmask_add_pc
     5.5%    .vcmul_quad_loop_start
     1.6%    DcRemovalFilter_f32
     5.9%    AddSamples
     3.4%    LryaStatsAccumulator

  6.2%     libm
    66.6%    atanf
    33.4%    __log_finite

  10.3%    libc
```

#### THREAD: wwise
```
      mate-anim (wwise)
```

#### THREAD: MicProcTrigger
```
27%      MicProcTrigger
  100%     mate-anim
    89.6%    u139y
     7%      b383i
     3.5%    w3e8m
```

## 5/8/2018 notes richard@lrya.com

# DVT2 - CPU performance

Note: idle means sitting on the charger for 2 minutes, shaken is free-play plus spoken at for 2 minutes

  - CLAHE_Interpolation_Body functions are gone
  - MicData is still more expensive than face rendering
  - 38% of face rendering is noise, 35% converting to HSV, 17% drawing eyes
  - malloc and new in the results
  - Dev_AssertIsValidParentPointer is still in the results
  - mate-cloud does not show results

## PROCESS: mate-engine

### Top 20 functions idle

```
Overhead  Command          Tid    Shared Object             Symbol
16.77%    VisionSystem     8761   libcozmo_engine           void Lrya::Embedded::ScrollingIntegralImage_u8_s32::FilterRow_innerLoop<unsigned char>(int, int, int, int, int const*, int const*, int const*, int const*, unsigned char*)
5.64%     VisionSystem     8761   libcozmo_engine           Lrya::Vision::ImageRGB::FillGray(Lrya::Vision::Image&) const
4.26%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ConnectedComponentsTemplate<unsigned short>::Extract2dComponents_PerRow_NextRow(unsigned char const*, int, short, short, short)
4.12%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ScrollingIntegralImage_u8_s32::ScrollDown(Lrya::Embedded::Array<unsigned char> const&, int, Lrya::Embedded::MemoryStack)
3.79%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ConnectedComponentsTemplate<unsigned short>::Extract1dComponents(unsigned char const*, short, short, short, Lrya::Embedded::FixedLengthList<Lrya::Embedded::ConnectedComponentSegment<unsigned short> >&)
2.72%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ecvcs_computeBinaryImage_numFilters3(Lrya::Embedded::Array<unsigned char> const&, Lrya::Embedded::FixedLengthList<Lrya::Embedded::Array<unsigned char> >&, int, int, unsigned char*, bool)
1.86%     CozmoRunner      8698   libcozmo_engine           Lrya::PoseBase<Lrya::Pose3d, Lrya::Transform3d>::PoseTreeNode::Dev_AssertIsValidParentPointer(Lrya::PoseBase<Lrya::Pose3d, Lrya::Transform3d>::PoseTreeNode const*, Lrya::PoseBase<Lrya::Pose3d, Lrya::Transform3d>::PoseTreeNode const*)
1.52%     CozmoRunner      8698   libcozmo_engine           Lrya::Cozmo::QuadTreeNode::Intersects(Lrya::FastPolygon const&) const
1.47%     CozmoRunner      8698   libc                      malloc
1.46%     CozmoRunner      8698   libcozmo_engine           std::__1::__function::__func<Lrya::Cozmo::QuadTree::Transform(std::__1::function<Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> > (Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >)>)::$_5, std::__1::allocator<Lrya::Cozmo::QuadTree::Transform(std::__1::function<Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> > (Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >)>)::$_5>, void (Lrya::Cozmo::QuadTreeNode&)>::operator()(Lrya::Cozmo::QuadTreeNode&)
1.34%     CozmoRunner      8698   libcozmo_engine           std::__1::__function::__func<Lrya::Cozmo::MemoryMap::GetBroadcastInfo(Lrya::Cozmo::MemoryMapTypes::MapBroadcastData&) const::$_12, std::__1::allocator<Lrya::Cozmo::MemoryMap::GetBroadcastInfo(Lrya::Cozmo::MemoryMapTypes::MapBroadcastData&) const::$_12>, void (Lrya::Cozmo::QuadTreeNode const&)>::operator()(Lrya::Cozmo::QuadTreeNode const&)
1.28%     CozmoRunner      8698   libcozmo_engine           std::__1::enable_if<std::is_base_of<Lrya::ConvexPolygon, Lrya::FastPolygon>::value, void>::type Lrya::Cozmo::QuadTreeNode::Fold<Lrya::FastPolygon>(std::__1::function<void (Lrya::Cozmo::QuadTreeNode&)>, Lrya::FastPolygon const&, Lrya::Cozmo::QuadTreeTypes::FoldDirection)
1.10%     VisionSystem     8761   libcozmo_engine           Lrya::Cozmo::RollingShutterCorrector::ComputePixelShiftsWithImageIMU(unsigned int, Lrya::Point<2u, float>&, Lrya::Cozmo::VisionPoseData const&, Lrya::Cozmo::VisionPoseData const&, float)
1.08%     CozmoRunner      8698   libcozmo_engine           Lrya::Cozmo::QuadTreeNode::Fold(std::__1::function<void (Lrya::Cozmo::QuadTreeNode&)>, Lrya::Cozmo::QuadTreeTypes::FoldDirection)
1.07%     VisionSystem     8761   libc                      memset
0.98%     CozmoRunner      8698   libpthread-2.22           pthread_mutex_lock
0.93%     CozmoRunner      8698   libcozmo_engine           std::__1::__function::__func<Lrya::Cozmo::QuadTree::Insert(Lrya::FastPolygon const&, std::__1::function<Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> > (Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >)>)::$_1, std::__1::allocator<Lrya::Cozmo::QuadTree::Insert(Lrya::FastPolygon const&, std::__1::function<Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> > (Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >)>)::$_1>, void (Lrya::Cozmo::QuadTreeNode&)>::operator()(Lrya::Cozmo::QuadTreeNode&)
0.92%     VisionSystem     8761   libc                      __clock_gettime
0.82%     CozmoRunner      8698   libcozmo_engine           @plt
0.77%     CozmoRunner      8698   libcozmo_engine           Lrya::FastPolygon::Contains(float, float) const
```

### Top 20 functions shaken

```
Overhead  Command          Tid    Shared Object             Symbol
11.40%    FaceRecognizer   8748   libcozmo_engine           OMR_F_FR80_0042
10.55%    VisionSystem     8761   libcozmo_engine           void Lrya::Embedded::ScrollingIntegralImage_u8_s32::FilterRow_innerLoop<unsigned char>(int, int, int, int, int const*, int const*, int const*, int const*, unsigned char*)
3.33%     FaceRecognizer   8748   libcozmo_engine           OMR_F_FR80_0049
2.78%     CozmoRunner      8698   libcozmo_engine           std::__1::__function::__func<Lrya::Cozmo::QuadTree::Transform(std::__1::function<Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> > (Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >)>)::$_5, std::__1::allocator<Lrya::Cozmo::QuadTree::Transform(std::__1::function<Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> > (Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >)>)::$_5>, void (Lrya::Cozmo::QuadTreeNode&)>::operator()(Lrya::Cozmo::QuadTreeNode&)
2.73%     VisionSystem     8761   libcozmo_engine           OMR_F_DT_0031
2.72%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ScrollingIntegralImage_u8_s32::ScrollDown(Lrya::Embedded::Array<unsigned char> const&, int, Lrya::Embedded::MemoryStack)
2.66%     CozmoRunner      8698   libcozmo_engine           Lrya::Cozmo::QuadTreeNode::Fold(std::__1::function<void (Lrya::Cozmo::QuadTreeNode&)>, Lrya::Cozmo::QuadTreeTypes::FoldDirection)
2.47%     VisionSystem     8761   libcozmo_engine           OMR_F_PT50_0003
2.37%     CozmoRunner      8698   libcozmo_engine           std::__1::__function::__func<Lrya::Cozmo::MemoryMap::GetBroadcastInfo(Lrya::Cozmo::MemoryMapTypes::MapBroadcastData&) const::$_12, std::__1::allocator<Lrya::Cozmo::MemoryMap::GetBroadcastInfo(Lrya::Cozmo::MemoryMapTypes::MapBroadcastData&) const::$_12>, void (Lrya::Cozmo::QuadTreeNode const&)>::operator()(Lrya::Cozmo::QuadTreeNode const&)
2.11%     FaceRecognizer   8748   libcozmo_engine           OMR_F_FR80_0056
2.11%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ConnectedComponentsTemplate<unsigned short>::Extract1dComponents(unsigned char const*, short, short, short, Lrya::Embedded::FixedLengthList<Lrya::Embedded::ConnectedComponentSegment<unsigned short> >&)
1.68%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ecvcs_computeBinaryImage_numFilters3(Lrya::Embedded::Array<unsigned char> const&, Lrya::Embedded::FixedLengthList<Lrya::Embedded::Array<unsigned char> >&, int, int, unsigned char*, bool)
1.47%     VisionSystem     8761   libopencv_imgproc         cv::ColumnSum<int, unsigned char>::operator()(unsigned char const**, unsigned char*, int, int, int)
1.24%     VisionSystem     8761   libcozmo_engine           Lrya::Embedded::ConnectedComponentsTemplate<unsigned short>::Extract2dComponents_PerRow_NextRow(unsigned char const*, int, short, short, short)
1.09%     FaceRecognizer   8748   libcozmo_engine           OMR_F_FR80_0054
1.06%     VisionSystem     8761   libcozmo_engine           OMR_F_DT_0048
1.04%     VisionSystem     8761   libopencv_imgproc         cv::resizeNNInvoker::operator()(cv::Range const&) const
1.03%     VisionSystem     8761   libcozmo_engine           Lrya::Vision::ImageRGB::FillGray(Lrya::Vision::Image&) const
1.03%     CozmoRunner      8698   libcozmo_engine           std::__1::__function::__func<Lrya::Cozmo::MapComponent::TimeoutObjects()::$_1, std::__1::allocator<Lrya::Cozmo::MapComponent::TimeoutObjects()::$_1>, Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> > (Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >)>::operator()(Lrya::Cozmo::MemoryMapDataWrapper<Lrya::Cozmo::MemoryMapData, std::__1::enable_if<true, void> >&&)
0.99%     FaceRecognizer   8748   libcozmo_engine           OMR_F_FR80_0041
```

### Hierarchical idle

#### THREAD: VisionSystem (51%)
```
  88% libcozmo_engine
    37% ScrollingIntegralImage_u8_s32::FilterRow_innerLoop
    12% FillGray
     9% ConnectedComponentsTemplate::Extract2dComponents_PerRow_NextRow
     9% ScrollingIntegralImage_u8_s32::ScrollDown
     8% ConnectedComponentsTemplate::Extract1dComponents_PerRow_NextRow
```

#### THREAD: CozmoRunner (47%)
```
  62% cozmo_engine
    17% other
    7% Dev_AssertDevIsValidParentPointer
    6% QuadTreeNode::Intersects
    5% QuadTreeNode::Transform
    5% MemoryMap::GetBroadcastInfo

  18% libc
    19% malloc
     6% free

   8% libc++
     8% new
     3% delete
```

### Hierarchical shaken

#### THREAD: VisionSystem (41% -10%)
```
  88% libcozmo_engine
    37% ScrollingIntegralImage_u8_s32::FilterRow_innerLoop
    12% FillGray
     9% ConnectedComponentsTemplate::Extract2dComponents_PerRow_NextRow
     9% ScrollingIntegralImage_u8_s32::ScrollDown
     8% ConnectedComponentsTemplate::Extract1dComponents_PerRow_NextRow
```

#### THREAD: CozmoRunner (37% -10%)
```
  65% cozmo_engine (+3%)
    17% other (=)
    14% QuadTreeNode::Transform (+9%)
    12% MemoryMap::GetBroadcastInfo (+9%)
    5% MapComponent::TimeoutObjects (NEW)
    4% Dev_AssertDevIsValidParentPointer (-3%)

  17% libc (-1%)
    17% malloc (-2%)
     7% free (+1%)
     7% memcpy
     5% vfprintf

  10% libc++ (+2%)
     4% new (-4%)
     2% delete (-1%)
```

#### THREAD: FaceRecognizer (20%)
```
  100% OMR_F_*
```

## PROCESS: mate-anim

### Top 20 functions idle

```
Overhead  Command          Tid    Shared Object             Symbol
36.70%    MicProcTrigger   8707   mate-anim                  u139y
6.12%     MicProcRaw       8706   mate-anim                  .i_loop_no_load
6.06%     mate-anim         8678   mate-anim                  Lrya::Cozmo::ProceduralFaceDrawer::ApplyNoise(Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB565 const&, Lrya::Vision::ImageRGB565&, Lrya::Cozmo::ProceduralFaceDrawer::DrawCacheState)
5.58%     mate-anim         8678   mate-anim                  Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
2.71%     MicProcRaw       8706   mate-anim                  VGenComplexTone_i16_ansi
2.63%     mate-anim         8678   mate-anim                  Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
2.05%     mate-anim         8678   libopencv_imgproc         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
1.96%     MicProcRaw       8706   libc                      memcpy
1.65%     MicProcTrigger   8707   mate-anim                  w3e8m
1.53%     MicProcTrigger   8707   mate-anim                  b383i
1.36%     MicProcRaw       8706   mate-anim                  ConvertFloatToInt16
1.13%     MicProcRaw       8706   mate-anim                  radf4_ps
1.09%     mate-anim         8678   libm                      roundf
0.99%     MicProcRaw       8706   mate-anim                  .qmask_add_pc
0.93%     MicProcRaw       8706   mate-anim                  .vcmul_quad_loop_start
0.91%     MicProcRaw       8706   mate-anim                  .vcrmul_quad_loop_start
0.91%     MicProcRaw       8706   mate-anim                  .loop_8_taps
0.83%     mate-anim         8678   libc                      memcpy
0.80%     MicProcRaw       8706   mate-anim                  VDotProductsLeftShift_q15_i16_ansi
```

### Top 20 functions shaken

```
Overhead  Command          Tid    Shared Object             Symbol
33.55%    MicProcTrigger   8707   mate-anim                  u139y
5.71%     MicProcRaw       8706   mate-anim                  .i_loop_no_load
4.90%     mate-anim         8678   mate-anim                  Lrya::Cozmo::ProceduralFaceDrawer::ApplyNoise(Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB565 const&, Lrya::Vision::ImageRGB565&, Lrya::Cozmo::ProceduralFaceDrawer::DrawCacheState)
4.58%     mate-anim         8678   mate-anim                  Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
2.81%     MicProcRaw       8706   mate-anim                  .loop_8
2.41%     MicProcRaw       8706   mate-anim                  VGenComplexTone_i16_ansi
2.19%     mate-anim         8678   libopencv_imgproc         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
2.19%     mate-anim         8678   mate-anim                  Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
1.76%     MicProcRaw       8706   libc                      memcpy
1.49%     MicProcTrigger   8707   mate-anim                  b383i
1.33%     MicProcTrigger   8707   mate-anim                  w3e8m
1.14%     MicProcRaw       8706   mate-anim                  ConvertFloatToInt16
1.09%     MicProcRaw       8706   mate-anim                  .qmask_add_pc
1.06%     MicProcRaw       8706   mate-anim                  radf4_ps
0.99%     MicProcRaw       8706   mate-anim                  .loop_8_taps
0.98%     mate-anim         8678   libm                      roundf
0.95%     MicProcRaw       8706   mate-anim                  .load_qmask
0.86%     MicProcRaw       8706   mate-anim                  .vcrmul_quad_loop_start
0.84%     MicProcRaw       8706   mate-anim                  .vcmul_quad_loop_start
0.82%     MicProcRaw       8706   mate-anim                  VDotProductsLeftShift_q15_i16_ansi
```

### Hierarchical idle

#### THREAD: MicProcTrigger (42%)
```
86% u139y
 4% w3e8m
```

#### THREAD: MicProcRaw (31%)
```
22% .i_loop_no_load
10% VGenComplextone_i16_ansi
```

#### THREAD: mate-anim (25%)
```
63% mate-anim
  38% ApplyNoise
  35% ConvertHSV
  17% DrawEye

15% opencv

 8% libc
```

### Hierarchical shaken

#### THREAD: MicProcTrigger (38% -3%)
```
88% u139y (+2%)
 4% w3e8m (=)
```

#### THREAD: MicProcRaw (32% ==)
```
89% mate-anim
  20% .i_loop_no_load (-2%)
  10% .loop_8 (NEW)
   8% VGenComplextone_i16_ansi (-2%)

 7% libc
   80% memcpy
```

#### THREAD: mate-anim (22% -3%)
```
60% mate-anim (-3%)
  37% ApplyNoise (-1%)
  35% ConvertHSV (=)
  17% DrawEye (=)

17% opencv (+2%)

 9% libc (+1%)
```

## PROCESS: mate-cloud

### Top 20 functions idle

```
Overhead  Command          Tid    Shared Object             Symbol
49.00%    mate-cloud        8684   mate-cloud                 mate-cloud[+5fde0]
25.93%    mate-cloud        8684   mate-cloud                 _start
19.77%    mate-cloud        8684   [vectors]                 [vectors][+fc8]
4.42%     mate-cloud        8684   [vectors]                 [vectors][+fc0]
0.84%     mate-cloud        8684   [vectors]                 [vectors][+fa0]
0.03%     mate-cloud        8684   mate-cloud                 mate-cloud[+5fe30]
```

### Top 20 functions shaken

```
Overhead  Command          Tid    Shared Object             Symbol
89.50%    mate-cloud        8684   mate-cloud                 _start
8.68%     mate-cloud        8684   [vectors]                 [vectors][+fd8]
1.80%     mate-cloud        8684   [vectors]                 [vectors][+fc0]
0.03%     mate-cloud        8684   mate-cloud                 mate-cloud[+5fe30]
```

## PROCESS: mate-robot

### Top 20 functions idle

```
Overhead  Command          Tid    Shared Object             Symbol
11.41%    mate-robot        8677   libgcc                    __divdi3
9.01%     mate-robot        8677   libc                      memcpy
7.45%     mate-robot        8677   mate-robot                 calc_crc
4.60%     mate-robot        8677   libc                      memset
3.69%     mate-robot        8677   libc                      __clock_gettime
2.25%     mate-robot        8677   mate-robot                 Lrya::Cozmo::spine_get_frame()
2.07%     mate-robot        8677   libc++                    std::__1::chrono::steady_clock::now()
1.87%     mate-robot        8677   mate-robot                 spine_parse_frame
1.80%     mate-robot        8677   mate-robot                 Lrya::Cozmo::IMUFilter::Update()
1.35%     mate-robot        8677   mate-robot                 @plt
1.29%     mate-robot        8677   libc                      read
1.24%     mate-robot        8677   mate-robot                 Lrya::Cozmo::IMUFilter::DetectPickup()
1.21%     mate-robot        8683   libc++                    std::__1::this_thread::sleep_for(std::__1::chrono::duration<long long, std::__1::ratio<1ll, 1000000000ll> > const&)
1.17%     mate-robot        8677   mate-robot                 Lrya::Cozmo::HAL::Step()
1.09%     mate-robot        8677   mate-robot                 GetTimeStamp
1.00%     mate-robot        8677   mate-robot                 Lrya::Cozmo::ProxSensors::UpdateCliff()
0.95%     mate-robot        8683   libc                      std::__1::chrono::steady_clock::now()
0.92%     mate-robot        8683   libc                      __clock_gettime
0.86%     mate-robot        8677   mate-robot                 Lrya::Cozmo::DockingController::Update()
0.80%     mate-robot        8677   mate-robot                 Lrya::Cozmo::Robot::step_MainExecution()
```

### Hierarchical idle

#### THREAD: mate-robot (87%)
```
49% mate-robot
  17% calc_crc
   5% spine_get_frame
   4% spine_parse_frame

27% libc
  38% memcpy
  19% memset

15% libc++
  87% __divdi3

5% libm
```

#### THREAD: mate-robot (12%)
```
30% mate-robot
  20% sleep
  20% ProcessIMUEvents
  16% spi_transfer
  15% imu_manage
  13% @plt

25% libc

23% libc++

16% libgcc
```

## 2/21/2018 notes richard@lrya.com

### DVT2 - CPU performance

  - Large single function consumers are working with bitmap images
  - Order of expensive functions doesnt change between idle and shaken, some additions
  - mate-anim process, mate-anim thread is consistent between idle and shaken
  - pthreads more prevalent than expected

### DVT1 - CPU performance

  - Large single function consumers are working with bitmap images
  - `_raw_spin_unlock_irq` and thread functions prevalent
  - many memory allocation related functions in sub-1% band
  - Hard to find relationship between DVT1 and Webots performance

### Webots - memory performance

  - finding the jpeg encoder does ~5000 allocations!
  - 1 persistent allocation
  - 300,880 transient allocations in 3.4GB 90% of which are `intel_performance_primitives`
  - `intel_performance_primitives` appears to be running on a different thread, IPC for allocations?

# DVT2 - CPU performance

## PROCESS: mate-engine

### Top 10 functions idle

```
Overhead  Command          Tid    Shared Object             Symbol
19.26%    VisionSystem     12260  libcozmo_engine        void Lrya::Embedded::ScrollingIntegralImage_u8_s32::FilterRow_innerLoop<unsigned char>(int, int, int, int, int const*, int const*, int const*, int const*, unsigned char*)
11.75%    VisionSystem     12260  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
6.21%     VisionSystem     12261  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
6.11%     VisionSystem     12260  libcozmo_engine        Lrya::Vision::Image::BoxFilter(Lrya::Vision::ImageBase<unsigned char>&, unsigned int) const
6.00%     VisionSystem     12263  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
5.80%     VisionSystem     12262  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
5.05%     VisionSystem     12260  libcozmo_engine        Lrya::Embedded::ScrollingIntegralImage_u8_s32::ScrollDown(Lrya::Embedded::Array<unsigned char> const&, int, Lrya::Embedded::MemoryStack)
3.76%     VisionSystem     12260  libcozmo_engine        Lrya::Embedded::ConnectedComponentsTemplate<unsigned short>::Extract1dComponents(unsigned char const*, short, short, short, Lrya::Embedded::FixedLengthList<Lrya::Embedded::ConnectedComponentSegment<unsigned short> >&)
3.15%     VisionSystem     12260  libcozmo_engine        Lrya::Embedded::ecvcs_computeBinaryImage_numFilters3(Lrya::Embedded::Array<unsigned char> const&, Lrya::Embedded::FixedLengthList<Lrya::Embedded::Array<unsigned char> >&, int, int, unsigned char*, bool)
1.71%     VisionSystem     12260  libcozmo_engine        Lrya::Vision::ImageRGB::FillGray(Lrya::Vision::Image&) const
```

### Top 10 functions shaken

```
Overhead  Command          Tid    Shared Object             Symbol
14.68%    VisionSystem     12260  libcozmo_engine        void Lrya::Embedded::ScrollingIntegralImage_u8_s32::FilterRow_innerLoop<unsigned char>(int, int, int, int, int const*, int const*, int const*, int const*, unsigned char*)
6.17%     VisionSystem     12260  libcozmo_engine        OMR_F_DT_0031
5.35%     VisionSystem     12260  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
5.24%     VisionSystem     12262  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
4.46%     VisionSystem     13166  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
4.27%     VisionSystem     12263  libopencv_imgproc      (anonymous namespace)::CLAHE_Interpolation_Body<unsigned char, 0>::operator()(cv::Range const&) const
3.98%     VisionSystem     12260  libcozmo_engine        Lrya::Vision::Image::BoxFilter(Lrya::Vision::ImageBase<unsigned char>&, unsigned int) const
3.54%     VisionSystem     12260  libcozmo_engine        Lrya::Embedded::ScrollingIntegralImage_u8_s32::ScrollDown(Lrya::Embedded::Array<unsigned char> const&, int, Lrya::Embedded::MemoryStack)
3.01%     CozmoRunner      12205  libcozmo_engine        Lrya::LineSegment::IntersectsWith(Lrya::LineSegment const&) const
2.73%     VisionSystem     12260  libcozmo_engine        Lrya::Embedded::ConnectedComponentsTemplate<unsigned short>::Extract1dComponents(unsigned char const*, short, short, short, Lrya::Embedded::FixedLengthList<Lrya::Embedded::ConnectedComponentSegment<unsigned short> >&)
```

### Hierarchical idle

#### THREAD: VisionSystem (64.1%)
```
  69.8% libcozmo_engine
    43.5% Lrya::Embedded...FilterRow_innerloop
    13.8% Lrya::Vision::Image::BoxFilter
    11.4% Lrya::Embedded...ScrollDown
     8.5% ConnectedComponentsTemplate::Extract1Dcomponents
     7.1% ecvcs_computeBinaryImage_numFilters3

  20.5% libopencv_imgproc
    89.9% CLAHE_Interpolation_Body
     8.3% CLAHE_CalcLut_Body

  4.7%  libc
    54.2% memset
    22.4% memcpy
     7.9% pthread_getspecific
```

#### THREAD: CozmoRunner (12.9%)
```
  60.3% libcozmo_engine
    19.8% Lrya::LineSegment::IntersectsWith
    10.7% Lrya::QuadTreeNode::GetOverlapType
     7.6% Lrya::PoseBase::DevAssert_IsValidParentPointer

  24.4% libc
    24.4% pthread_mutex_unlock
    18.4% pthread_mutex_lock
     9.8% pthread_getspecific
     7.4% @plt

   8.6% libc++
    38.2% __release_shared
```

#### THREAD: VisionSystem (7.8%)
```
  ... CLAHE_Interpolation_Body
```

#### THREAD: VisionSystem (7.6%)
```
  ... CLAHE_Interpolation_Body
```

#### THREAD: VisionSystem (7.4%)
```
  ... CLAHE_Interpolation_Body
```

### Hierarchical shaken

#### THREAD: VisionSystem (58.0% < 64.1%)
```
  79.8% libcozmo_engine (> 69.8%)
    32.1% Lrya::Embedded...FilterRow_innerloop (< 43.5%)
    13.5% OMR_F_DT_0031 ()
     8.7% Lrya::Vision::Image::BoxFilter (< 13.8%)
     7.7% Lrya::Embedded...ScrollDown (< 11.4%)
     6.0% ConnectedComponentsTemplate::Extract1Dcomponents (8.5%)
     5.6% OMR_F_DT_0048 ()
     4.8% ecvcs_computeBinaryImage_numFilters3 (7.1%)

  13.8% libopencv_imgproc (< 20.5%)
    67.3% CLAHE_Interpolation_Body (< 89.9%)
     7.1% cv::ColumnSum ()
     5.9% cv::resizeNNInvoker ()
     4.9% CLAHE_CalcLut_Body (<8.3%)

  3.5% libc (<4.7%)
    53.0% memset (<54.2%)
    27.5% memcpy (>22.4%)
     5.1% pthread_getspecific (<7.9%)
```

#### THREAD: CozmoRunner (22.7% > 12.9%)
```
  61.3% libcozmo_engine (~= 60.3%)
    19.8% Lrya::LineSegment::IntersectsWith
    10.7% Lrya::QuadTreeNode::GetOverlapType
     7.6% Lrya::PoseBase::DevAssert_IsValidParentPointer

  21.4% libc (~= 24.4%)
    18.7% pthread_mutex_unlock (< 24.4%)
    13.3% pthread_mutex_lock (< 18.4%)
     8.7% pthread_getspecific (~= 9.8%)
     6.2% @plt (~= 7.4%)

   9.8% libc++ (~= 8.6%)
    43.2% __release_shared (> 38.2%)
```

#### THREAD: VisionSystem (6.5% < 7.8%)
```
  ... CLAHE_Interpolation_Body
```

#### THREAD: VisionSystem (5.6% < 7.6%)
```
  ... CLAHE_Interpolation_Body
```

#### THREAD: VisionSystem (5.3% < 7.4%)
```
  ... CLAHE_Interpolation_Body
```

## PROCESS: mate-anim

### Top 10 functions idle

```
Overhead  Command          Tid    Shared Object             Symbol
11.74%    MicDataProc      12190  mate-anim                  .i_loop_no_load
8.74%     MicDataProc      12190  mate-anim                  u139y
6.10%     mate-anim         12183  mate-anim                  Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
4.87%     MicDataProc      12190  mate-anim                  VGenComplexTone_i16_ansi
4.20%     mate-anim         12183  mate-anim                  Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
3.55%     mate-anim         12183  libopencv_imgproc      void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
3.50%     MicDataProc      12190  libc                   memcpy
2.71%     MicDataProc      12190  mate-anim                  ConvertFloatToInt16
2.69%     MicDataProc      12190  mate-anim                  radf4_ps
2.25%     mate-anim         12183  libm                   floorf
```

### Top 10 functions shaken

```
Overhead  Command          Tid    Shared Object             Symbol
47.78%    MicDataProc      12190  mate-anim                  u139y
4.85%     MicDataProc      12190  mate-anim                  .i_loop_no_load
3.35%     mate-anim         12183  mate-anim                  Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
2.46%     MicDataProc      12190  mate-anim                  .loop_8
2.38%     mate-anim         12183  mate-anim                  Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
2.23%     MicDataProc      12190  mate-anim                  a562j
2.03%     mate-anim         12183  libopencv_imgproc      void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
1.99%     MicDataProc      12190  mate-anim                  VGenComplexTone_i16_ansi
1.65%     MicDataProc      12190  mate-anim                  b383i
1.54%     MicDataProc      12190  libc                   memcpy
```

### Hierarchical idle

#### THREAD: MicDataProc (64%)
```
90% mate-anim
  20.3% i_loop_no_load
  15.1% u139y
   8.4% VGenComplexTone_i16_ansi
   4.7% ConvertFloatToInt16

7.5% libc
  78.0% memcpy
   3.9% memset
```

#### THREAD: mate-anim (34.7%)
```
38.4% mate-anim
  47.3% DrawEye
  32.6% ConvertHSV2RGB565
   7.9% ApplyScanlines
   4.0% @plt

28.5% libopencv_imgproc
  35.9% cv::remapNearest
  13.6% cv::ColumnSum
  11.4% cv::RowSum
  10.7% cv::LineAA

13.4% libm
  48.0% floorf
  42.1% roundf
   8.0% @plt

12.4% libc
  48.6% memcpy
   7.2% pthread_mutex_lock
   6.8% pthread_mutex_unlock
```

### Hierarchical shaken

#### THREAD: MicDataProc (79.9% > 64%)
```
96.7% mate-anim (> 90%)
  62.2% u139y (> 15.1%)
   6.3% i_loop_no_load (< 20.3%)
   4.4% <other>
   3.2% loop_8
   2.6% VGenComplexTone_i16_ansi (< 8.4%)
   1.4% ConvertFloatToInt16 (4.7%)

2.6% libc (< 7.5%)
  81.9% memcpy (> 78.0%)
   4.1% memset (> 3.9%)
```

#### THREAD: mate-anim (19.9% < 34.7%)
```
38.4% mate-anim (== 38.4%)
  48.3% DrawEye (~= 47.3%)
  34.4% ConvertHSV2RGB565 (~= 32.6%)
   8.3% ApplyScanlines (~= 7.9%)
   3.9% @plt (== 4.0%)

28.4% libopencv_imgproc (== 28.5%)
  38.1% cv::remapNearest (> 35.9%)
  12.4% cv::ColumnSum (> 13.6%)
  10.9% cv::RowSum (< 11.4%)
  11.5% cv::LineAA (< 10.7%)

14.1% libm (~= 13.4%)
  49.6% floorf (~= 48.0%)
  41.4% roundf (~= 42.1%)
   7.7% @plt (~= 8.0%)

11.9% libc (~= 12.4%)
  55.8% memcpy (> 48.6%)
   7.9% pthread_mutex_lock (> 7.2%)
   7.1% pthread_mutex_unlock (> 6.8%)
   7.1% pthread_getspecific ()
```

# DVT1 - CPU performance

## PROCESS: cozmoengined

#### THREAD: VisionSystem
```
  23%    Lrya::Embedded::ScrollingIntegralImage_u8_s32::FilterRow_innerLoop
  11%    CLAHE_Interpolation_Body
   7%    Lrya::Embedded::ScrollingIntegralImage_u8_s32::ScrollDown
   6%    Lrya::Embedded::ExtractLineFitsPeaks
   5%    Lrya::Embedded::ConnectedComponentsTemplate
   3%    Lrya::Embedded::ecvcs_computeBinaryImage_numFilters3
   2%    Lrya::Vision::ImageRGB::FillGray
   1%    CLAHE_CalcLut_Body
   1%    Lrya::Embedded::ScrollingIntegralImage_u8_s32::PadImageRow
```

#### THREAD: CozmoRunner
```
   3%    Lrya::Cozmo::UiMessageHandler::Update()
   3%    je_arena_malloc_hard
   3%    arena_dalloc_bin_locked_impl
   2%    je_malloc
   2%    arena_run_reg_alloc
   1%    unlock 
   1%    Lrya::PoseBase<Lrya::Pose3d, Lrya::Transform3d>::PoseTreeNode::Dev_AssertIsValidParentPointer
```

#### THREAD: VisionSystem
```
  61%    CLAHE_Interpolation_Body
  20%    _raw_spin_unlock_irq
   7%    CLAHE_CalcLut_Body
   2%    cv::KMeansDistanceComputer::operator()
```

#### THREAD: VisionSystem
```
  62%    CLAHE_Interpolation_Body
  17%    _raw_spin_unlock_irq
   6%    CLAHE_CalcLut_Body
   2%    cv::KMeansDistanceComputer::operator()
```

#### THREAD: VisionSystem
```
  63%    CLAHE_Interpolation_Body
  15%    _raw_spin_unlock_irq
   7%    CLAHE_CalcLut_Body
   2%    cv::KMeansDistanceComputer::operator()
```

#### THREAD: mate-engine
```
  48%    _raw_spin_unlock_irqrestore
   7%    usleep
   2.5%  IsRunning
```

#### THREAD: mm_cam_stream
```
  75%    mm_app_snapshot_notify_cb_raw
  13%    v7_dma_inv_range
```

#### THREAD: FaceRecognizer
```
  40%    _raw_spin_unlock_irqrestore
   4%    FaceRecognizer::Run
   7%    sleep
   2%    thread_mutex_lock
   1%    thread_mutex_unlock
```

#### THREAD: BehaviorServer
```
  36%    _raw_spin_unlock_irqrestore
  11%    mutex
```

#### THREAD: civetweb-master
```
  82%    poll
```

#### THREAD: camera?
```
  51%    v7_dma_flush_range
```


## PROCESS: mate-anim

#### THREAD: MicDataProc
```
  26%    _raw_spin_unlock_irqrestore
   8%    Lrya::Cozmo::MicData::MicDataProcessor::ProcessLoop
   4%    sleep
```

#### THREAD: mate-anim
```
  18%    cv::HSV2RGB...
  11%    cv::remapNearest
   9%    Lrya::Cozmo::ProceduralFaceDrawer::DrawEye
   6%    cv_vrndq_u32_f32
   6%    memcpy
   3%    cv::LineAA
   3%    cv::WarpAffineInvoker::operator()
   2%    floor
   2%    round
   2%    cv::RGB2RGB5x5::operator()
   1%    cv::fastMalloc
```

#### THREAD: DrawFaceLoop
```
  75%    write
  17%    sleep
   2%    Lrya::Cozmo::FaceDisplay::DrawFaceLoop()
```

#### THREAD: civetweb-master
```
  86%    poll
   6%    master_thread
```

#### THREAD: wwise
```
  36%    sleep
```

#### THREAD: wwise
```
  32%    sleep
```

# Webots - CPU performance

## PROCESS: cozmoengined

### THREAD: main thread
```
  99%    CozmoAPI::Update
    87%    Robot::Update
      70%    VisionComponent::Update
        47%    VisionComponent::Update::CaptureImage
        22%    VisionComponent::CompressAndSendImage
    11%    RobotManager::UpdateRobotConnection
```

#### THREAD: VISIONPROCESSORTHREAD
```
  86%    UpdateVisionSystem
  ...
  76%    Lrya::Embedded::DetectFiducialMarkers
  54%    Lrya::Embedded::ExtraComponentsViaCharacterIsticScale

  ultimately ucvcs_ functions
```

#### THREAD: TASKEXECUTORTHREAD
```
  57%    Lrya::Util::RollingFileLogger
    53%    ostream.flush
```

## PROCESS: mate-anim

#### THREAD: MAIN THREAD
```
  51%    AnimationStreamer::Update
    47%    AnimationStreamer::StreamLayers
    45%    AnimationStreamer::BufferFaceToSend
  31%    AnimProcessMessages::Update
    13%    GetNextPacketFromRobot => UDP
    10%    ProcessMessages => UDP
     4%    MicDataProcessor
```

#### THREAD: MIC PROCESSOR
```
  38%    MicDataProcessor::ProcessResampledAudio
  35%    MicDataProcessor::ResampleAudioChunk
  23%    SpeechRecognizerTHF
```

#### THREAD: WWISE

#### THREAD: FACE DISPLAY
```
  65%   sleeping
```

# Webots - memory performance

#### Cozmo process
```
  69%,         422MB, 1910594   Cozmo::Robot::UpdateAllRobots
    68%,       420MB, 1885073     Cozmo::Robot::Update
    48%,       298MB,  160492       Cozmo::VisionComponent::Update
      26%,     163MB,   63840         Cozmo::VisionComponent::CompressAndSendImage
        19.3%, 117MB,   23940           cv::JpegEncoder
         9.0%, 274KB,    4788           cv::findEncoder
    16%,       100MB, 1478710       Cozmo::VisionComponent::UpdateAllResults
     6%,        40MB,  528276         CreateObjectsFromMarkers
     5%,        30MB,  489172         AddAndUpdateObjects
     2.5%,      15MB,  239400         UpdatePoseOfStackedObjcets
  13%,          99MB,  1508447  Cozmo::Robot::UpdateRobotConnection
```

#### Anim process
```
  90%,         3.2GB, 196785    ippMalloc
   4%,         156MB,  61374    Cozmo::AnimEngine::Update
```

# Performance costs of features in face rendering

Sampled over 60 seconds on DVT2, Victor was shaken.

Key             |Options
----------------|-------
COZMO RENDERING | none of the new Victor features
DESKMATE RENDERING| all of the Victor features enabled
X-glowfilt      | all Victor features enabled except kProcFace_ApplyGlowFilter
X-aliasing      | all Victor features enabled except kProcFace_AntiAliasingSize
X-outerglow     | all Victor features enabled except kProcFace_RenderInnerOuterGlow
X-noise         | all Victor features enabled except kProcFace_UseNoise

## Notes

mate-anim process is dominated by the MicDataThread, 47%.
Cozmo rendering costs 2.35% per eye
Victor rendering costs 4.87% per eye
OpenCV filtering costs ~2%
Worst case cost ~22% (4.87% + 4.87% + 3.75% + 2.97% + 1.83% + 1.62% + 1.09% + 0.87% + 0.87%)

## Suggestions

ApplyScanlines NEONized
Test rendering quality with lines AA or not
Triangle rasterization NEONized

```
47.07%    DESKMATE RENDERING X-glowfilt   MicDataProc      u139y
46.90%    DESKMATE RENDERING X-aliasing   MicDataProc      u139y
44.54%    DESKMATE RENDERING X-outerglow  MicDataProc      u139y
40.33%    COZMO RENDERING               MicDataProc      u139y
35.11%    DESKMATE RENDERING X-noise      MicDataProc      u139y
34.35%    DESKMATE RENDERING              MicDataProc      u139y

6.17%     COZMO RENDERING               MicDataProc      .i_loop_no_load
5.88%     DESKMATE RENDERING              MicDataProc      .i_loop_no_load
5.76%     DESKMATE RENDERING X-noise      MicDataProc      .i_loop_no_load
5.48%     DESKMATE RENDERING X-outerglow  MicDataProc      .i_loop_no_load
5.28%     DESKMATE RENDERING X-glowfilt   MicDataProc      .i_loop_no_load
4.77%     DESKMATE RENDERING X-aliasing   MicDataProc      .i_loop_no_load

4.87%     DESKMATE RENDERING X-noise      mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
4.81%     DESKMATE RENDERING              mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
2.72%     DESKMATE RENDERING X-aliasing   mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
2.47%     DESKMATE RENDERING X-outerglow  mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
2.39%     DESKMATE RENDERING X-glowfilt   mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)
2.35%     COZMO RENDERING               mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::DrawEye(Lrya::Cozmo::ProceduralFace const&, Lrya::Cozmo::ProceduralFace::WhichEye, Lrya::Util::RandomGenerator const&, Lrya::Vision::ImageRGB&, Lrya::Rectangle<float>&)

3.75%     COZMO RENDERING               mate-anim         Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
3.48%     DESKMATE RENDERING X-noise      mate-anim         Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
3.44%     DESKMATE RENDERING              mate-anim         Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
2.84%     DESKMATE RENDERING X-outerglow  mate-anim         Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
2.47%     DESKMATE RENDERING X-glowfilt   mate-anim         Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)
2.37%     DESKMATE RENDERING X-aliasing   mate-anim         Lrya::Vision::ImageRGB::ConvertHSV2RGB565(Lrya::Vision::ImageRGB565&)

2.97%     COZMO RENDERING               mate-anim         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
2.78%     DESKMATE RENDERING X-noise      mate-anim         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
2.69%     DESKMATE RENDERING              mate-anim         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
2.09%     DESKMATE RENDERING X-glowfilt   mate-anim         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
2.06%     DESKMATE RENDERING X-outerglow  mate-anim         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)
2.04%     DESKMATE RENDERING X-aliasing   mate-anim         void cv::remapNearest<unsigned char>(cv::Mat const&, cv::Mat&, cv::Mat const&, int, cv::Scalar_<double> const&)

1.83%     DESKMATE RENDERING X-noise      mate-anim         floorf
1.76%     DESKMATE RENDERING              mate-anim         floorf
1.04%     DESKMATE RENDERING X-outerglow  mate-anim         floorf
0.90%     COZMO RENDERING               mate-anim         floorf
0.85%     DESKMATE RENDERING X-aliasing   mate-anim         floorf
0.74%     DESKMATE RENDERING X-glowfilt   mate-anim         floorf

1.62%     DESKMATE RENDERING              mate-anim         roundf
1.60%     DESKMATE RENDERING X-noise      mate-anim         roundf
0.89%     DESKMATE RENDERING X-outerglow  mate-anim         roundf
0.73%     DESKMATE RENDERING X-aliasing   mate-anim         roundf
0.71%     COZMO RENDERING               mate-anim         roundf
0.62%     DESKMATE RENDERING X-glowfilt   mate-anim         roundf

1.09%     DESKMATE RENDERING X-noise      mate-anim         cv::ColumnSum<unsigned short, unsigned char>::operator()(unsigned char const**, unsigned char*, int, int, int)
1.05%     DESKMATE RENDERING              mate-anim         cv::ColumnSum<unsigned short, unsigned char>::operator()(unsigned char const**, unsigned char*, int, int, int)
0.76%     DESKMATE RENDERING X-outerglow  mate-anim         cv::ColumnSum<unsigned short, unsigned char>::operator()(unsigned char const**, unsigned char*, int, int, int)
0.38%     DESKMATE RENDERING X-outerglow  mate-anim         cv::ColumnSum<int, unsigned char>::operator()(unsigned char const**, unsigned char*, int, int, int)
0.37%     DESKMATE RENDERING X-aliasing   mate-anim         cv::ColumnSum<unsigned short, unsigned char>::operator()(unsigned char const**, unsigned char*, int, int, int)
0.36%     DESKMATE RENDERING X-glowfilt   mate-anim         cv::ColumnSum<unsigned short, unsigned char>::operator()(unsigned char const**, unsigned char*, int, int, int)

0.87%     COZMO RENDERING               mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::ApplyScanlines(Lrya::Vision::ImageRGB&, float)
0.80%     DESKMATE RENDERING X-noise      mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::ApplyScanlines(Lrya::Vision::ImageRGB&, float)
0.80%     DESKMATE RENDERING              mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::ApplyScanlines(Lrya::Vision::ImageRGB&, float)
0.60%     DESKMATE RENDERING X-outerglow  mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::ApplyScanlines(Lrya::Vision::ImageRGB&, float)
0.60%     DESKMATE RENDERING X-glowfilt   mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::ApplyScanlines(Lrya::Vision::ImageRGB&, float)
0.60%     DESKMATE RENDERING X-aliasing   mate-anim         Lrya::Cozmo::ProceduralFaceDrawer::ApplyScanlines(Lrya::Vision::ImageRGB&, float)

0.87%     COZMO RENDERING               mate-anim         cv::LineAA(cv::Mat&, cv::Point_<long long>, cv::Point_<long long>, void const*)
0.80%     DESKMATE RENDERING              mate-anim         cv::LineAA(cv::Mat&, cv::Point_<long long>, cv::Point_<long long>, void const*)
0.78%     DESKMATE RENDERING X-noise      mate-anim         cv::LineAA(cv::Mat&, cv::Point_<long long>, cv::Point_<long long>, void const*)
0.63%     DESKMATE RENDERING X-outerglow  mate-anim         cv::LineAA(cv::Mat&, cv::Point_<long long>, cv::Point_<long long>, void const*)
0.62%     DESKMATE RENDERING X-glowfilt   mate-anim         cv::LineAA(cv::Mat&, cv::Point_<long long>, cv::Point_<long long>, void const*)
0.61%     DESKMATE RENDERING X-aliasing   mate-anim         cv::LineAA(cv::Mat&, cv::Point_<long long>, cv::Point_<long long>, void const*)

0.73%     COZMO RENDERING               mate-anim         cv::WarpAffineInvoker::operator()(cv::Range const&) const
0.68%     DESKMATE RENDERING              mate-anim         cv::WarpAffineInvoker::operator()(cv::Range const&) const
0.65%     DESKMATE RENDERING X-noise      mate-anim         cv::WarpAffineInvoker::operator()(cv::Range const&) const
0.49%     DESKMATE RENDERING X-glowfilt   mate-anim         cv::WarpAffineInvoker::operator()(cv::Range const&) const
0.49%     DESKMATE RENDERING X-aliasing   mate-anim         cv::WarpAffineInvoker::operator()(cv::Range const&) const
0.48%     DESKMATE RENDERING X-outerglow  mate-anim         cv::WarpAffineInvoker::operator()(cv::Range const&) const

0.97%     DESKMATE RENDERING X-noise      mate-anim         memcpy
0.95%     DESKMATE RENDERING              mate-anim         memcpy
0.89%     COZMO RENDERING               mate-anim         memcpy
0.74%     DESKMATE RENDERING X-outerglow  mate-anim         memcpy
0.67%     DESKMATE RENDERING X-aliasing   mate-anim         memcpy
0.65%     DESKMATE RENDERING X-glowfilt   mate-anim         memcpy

0.32%     DESKMATE RENDERING              mate-anim         cv::FilterEngine::proceed(unsigned char const*, int, int, unsigned char*, int)
0.27%     DESKMATE RENDERING X-outerglow  mate-anim         cv::FilterEngine::proceed(unsigned char const*, int, int, unsigned char*, int)

0.90%     DESKMATE RENDERING X-noise      mate-anim         cv::RowSum<unsigned char, unsigned short>::operator()(unsigned char const*, unsigned char*, int, int)
0.90%     DESKMATE RENDERING              mate-anim         cv::RowSum<unsigned char, unsigned short>::operator()(unsigned char const*, unsigned char*, int, int)
0.65%     DESKMATE RENDERING X-outerglow  mate-anim         cv::RowSum<unsigned char, unsigned short>::operator()(unsigned char const*, unsigned char*, int, int)
0.38%     DESKMATE RENDERING X-aliasing   mate-anim         cv::RowSum<unsigned char, unsigned short>::operator()(unsigned char const*, unsigned char*, int, int)
0.25%     DESKMATE RENDERING X-glowfilt   mate-anim         cv::RowSum<unsigned char, unsigned short>::operator()(unsigned char const*, unsigned char*, int, int)
```
