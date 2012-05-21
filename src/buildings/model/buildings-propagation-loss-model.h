/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Marco Miozzo  <marco.miozzo@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 * 
 */

#ifndef BUILDINGS_PROPAGATION_LOSS_MODEL_H_
#define BUILDINGS_PROPAGATION_LOSS_MODEL_H_

#include "ns3/nstime.h"
#include "ns3/propagation-loss-model.h"
#include <ns3/building.h>
#include <ns3/buildings-mobility-model.h>


namespace ns3 {

class ShadowingLossModel;
class JakesFadingLossModel;

/**
 * \ingroup propagation
 *
 *  This model provides means for simulating the following propagation
 *  phenomena in the presence of buildings: 
 *
 *   - shadowing (indoor, outdoor)
 *   - external wall penetration loss
 *   - internal wall penetration loss
 *  
 *  The distance-dependent component of propagation loss is deferred
 *  to derived classes which are expected to implement the GetLoss method.
 *  
 *  \warning This model works only with BuildingsMobilityModel
 *
 */

class BuildingsPropagationLossModel : public PropagationLossModel
{

public:
  static TypeId GetTypeId (void);

  /**
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the propagation loss (in dBm)
   */
  virtual double GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const = 0;

  // inherited from PropagationLossModel
  virtual double DoCalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;

protected:

  double ExternalWallLoss (Ptr<BuildingsMobilityModel> a) const;
  double HeightLoss (Ptr<BuildingsMobilityModel> n) const;
  double InternalWallsLoss (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
  
  double GetShadowing (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;

  double m_lossInternalWall; // in meters

  
  class ShadowingLoss
  {
  public:
    ShadowingLoss ();
    ShadowingLoss (double mean, double sigma, Ptr<MobilityModel> receiver);
    double GetLoss () const;
    Ptr<MobilityModel> GetReceiver (void) const;
  protected:
    Ptr<MobilityModel> m_receiver;
    NormalVariable m_randVariable;
    double m_shadowingValue;
  };

  mutable std::map<Ptr<MobilityModel>,  std::map<Ptr<MobilityModel>, ShadowingLoss> > m_shadowingLossMap;
  double EvaluateSigma (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;


  double m_shadowingSigmaExtWalls;
  double m_shadowingSigmaOutdoor;
  double m_shadowingSigmaIndoor;

};

}

#endif /* BUILDINGS_PROPAGATION_LOSS_MODEL_H_ */
