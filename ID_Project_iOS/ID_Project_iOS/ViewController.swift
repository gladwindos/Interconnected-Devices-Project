//
//  ViewController.swift
//  ID_Project_iOS
//
//  Created by Gladwin Dosunmu on 12/12/2018.
//  Copyright © 2018 Gladwin. All rights reserved.
//

import UIKit
import CocoaMQTT

class ViewController: UIViewController, CocoaMQTTDelegate {
    
    @IBOutlet var tempVal: UILabel!
    
    @IBOutlet var lightVal: UILabel!
    
    var mqtt: CocoaMQTT!

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        tempVal.text = ""
        lightVal.text = ""
        
        setUpMQTT()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func setUpMQTT() {
        
        let clientID = "CocoaMQTT-" + String(ProcessInfo().processIdentifier)
        
        mqtt = CocoaMQTT(clientID: clientID, host: "m23.cloudmqtt.com", port: 28368)
        mqtt.username = "tqvjzgfc"
        mqtt.password = "kgKYC5nzmFUP"
        mqtt.willMessage = CocoaMQTTWill(topic: "/will", message: "dieout")
        mqtt.keepAlive = 60
        mqtt.enableSSL = true
        mqtt.allowUntrustCACertificate = true
        mqtt.connect()
        
        mqtt.delegate = self
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didReceiveMessage message: CocoaMQTTMessage, id: UInt16 ) {
        print("Message Recieved")
        if let msg = message.string {
            switch message.topic {
            case "project/temp":
                tempVal.text = msg + "°C"
            case "project/light":
                lightVal.text = msg
            default:
                print("Unkown topic")
            }
        }
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didReceive trust: SecTrust, completionHandler: @escaping (Bool) -> Void) {
        completionHandler(true)
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didConnectAck ack: CocoaMQTTConnAck) {
        print("ConnectedAK!")
        mqtt.subscribe("project/temp")
        mqtt.subscribe("project/light")

    }
    
    func mqtt(_ mqtt: CocoaMQTT, didPublishMessage message: CocoaMQTTMessage, id: UInt16) {
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didPublishAck id: UInt16) {
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didSubscribeTopic topic: String) {
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didUnsubscribeTopic topic: String) {
    }
    
    func mqttDidPing(_ mqtt: CocoaMQTT) {
    }
    
    func mqttDidReceivePong(_ mqtt: CocoaMQTT) {
    }
    
    func mqttDidDisconnect(_ mqtt: CocoaMQTT, withError err: Error?) {
        print(err.debugDescription)
        mqtt.connect()
    }
    
    func _console(_ info: String) {
    }
}
