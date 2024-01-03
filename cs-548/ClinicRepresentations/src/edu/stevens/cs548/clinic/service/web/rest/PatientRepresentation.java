package edu.stevens.cs548.clinic.service.web.rest;

import java.util.List;

import javax.ws.rs.core.UriBuilder;
import javax.ws.rs.core.UriInfo;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlRootElement;

import edu.stevens.cs548.clinic.service.dto.PatientDTO;
import edu.stevens.cs548.clinic.service.web.rest.data.LinkType;
import edu.stevens.cs548.clinic.service.web.rest.data.PatientType;

@XmlRootElement(name="patientRepresentation")
@XmlAccessorType(XmlAccessType.FIELD)
public class PatientRepresentation extends PatientType {
    
    
    public List<LinkType> getLinksTreatments() {
        return this.getTreatments();
    }
    
    public PatientRepresentation() {
        super();
    }
    
    
    public PatientRepresentation(PatientDTO patientDTO, UriInfo uriInfo) {
        this.id = patientDTO.getId();
        this.patientId = patientDTO.getPatientId();
        this.name = patientDTO.getName();
        this.dob = patientDTO.getDob();
        
        List<Long> treatments = patientDTO.getTreatments();
        UriBuilder ub = uriInfo.getBaseUriBuilder();
        ub.path("treatment");
        
        List<LinkType> links = this.getTreatments();
        for(long t: treatments) {
            LinkType link = new LinkType();
            UriBuilder ubTreatment = ub.clone().path("{tid}");
            String treatmentURI = ubTreatment.build(Long.toString(t)).toString();
            link.setUrl(treatmentURI);
            link.setRelation(Representation.RELATION_TREATMENT);
            link.setMediaType(Representation.MEDIA_TYPE);
            links.add(link);
        }
    }
    
    // just for test
    public PatientRepresentation(PatientDTO patientDTO) {
        this.id = patientDTO.getId();
        this.patientId = patientDTO.getPatientId();
        this.name = patientDTO.getName();
        this.dob = patientDTO.getDob();
        
        List<Long> treatments = patientDTO.getTreatments();
        
        List<LinkType> links = this.getTreatments();
        for(long t: treatments) {
            LinkType link = new LinkType();
            String treatmentURI = Long.toString(t);
            link.setUrl(treatmentURI);
            link.setRelation(Representation.RELATION_TREATMENT);
            link.setMediaType(Representation.MEDIA_TYPE);
            links.add(link);
        }
    }
}
