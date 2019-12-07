import Foundation
import CoreLocation
import MapKit
import UIKit
import CoreBluetooth


class ViewController : UIViewController, CLLocationManagerDelegate, CBPeripheralDelegate  {
    
    @IBOutlet weak var map: MKMapView!
    @IBOutlet weak var longitude: UILabel!
    @IBOutlet weak var latitude: UILabel!
    @IBOutlet weak var heading: UILabel!
    
    var locationManager: CLLocationManager!
    
    
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        
        if (CLLocationManager.locationServicesEnabled())
        {
            locationManager = CLLocationManager()
            locationManager.delegate = self
            locationManager.desiredAccuracy = kCLLocationAccuracyBest
            locationManager.requestAlwaysAuthorization()
            locationManager.startUpdatingLocation()
            locationManager.startUpdatingHeading()
        }
    }
    
    func locationManager(_ manager: CLLocationManager, didUpdateHeading newHeading: CLHeading) {
        let h2 = round(newHeading.trueHeading * 1)/1 // -1 if no location info, rounded to integer
        
        heading.text = "heading \(h2)"
    }
    
    func locationManager(_ _manager: CLLocationManager, didUpdateLocations locations: [CLLocation]
    )
    {
        
        let location = locations.last! as CLLocation
        
        
        let center = CLLocationCoordinate2D(latitude: location.coordinate.latitude, longitude: location.coordinate.longitude)
        let region = MKCoordinateRegion(center: center, span: MKCoordinateSpan(latitudeDelta: 0.01, longitudeDelta: 0.01))
        
        var currentLocation: CLLocation!
        
        if( CLLocationManager.authorizationStatus() == .authorizedWhenInUse ||
            CLLocationManager.authorizationStatus() ==  .authorizedAlways){
            
            currentLocation = locationManager.location
        }
        self.map.setRegion(region, animated: true)
        
        longitude.text = "Longitude \(round(currentLocation.coordinate.longitude * 10000000) / 10000000)"
        latitude.text = "Latitude \(round(currentLocation.coordinate.latitude * 10000000) / 10000000)"
    }
}
