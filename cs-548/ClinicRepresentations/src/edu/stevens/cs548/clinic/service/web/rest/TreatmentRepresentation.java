package edu.stevens.cs548.clinic.service.web.rest;

import javax.xml.bind.annotation.XmlRootElement;

import edu.stevens.cs548.clinic.service.web.rest.data.LinkType;
import edu.stevens.cs548.clinic.service.web.rest.data.TreatmentType;

@XmlRootElement
public class TreatmentRepresentation extends TreatmentType {
	
	public LinkType getLinkPatient() {
		return this.getPatient();
	}
	
	public LinkType getLinkProvider() {
		return this.getProvider();
	}
}
