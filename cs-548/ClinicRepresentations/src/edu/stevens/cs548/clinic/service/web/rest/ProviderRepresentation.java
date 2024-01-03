package edu.stevens.cs548.clinic.service.web.rest;

import java.util.List;

import javax.ws.rs.core.UriBuilder;
import javax.ws.rs.core.UriInfo;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlRootElement;

import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.web.rest.data.LinkType;
import edu.stevens.cs548.clinic.service.web.rest.data.ProviderType;
import edu.stevens.cs548.clinic.service.web.rest.data.SpeicalizationType;

@XmlRootElement(name="providerRepresentation")
@XmlAccessorType(XmlAccessType.FIELD)
public class ProviderRepresentation extends ProviderType {
    
    public List<LinkType> getLinksTreatments() {
        return this.getTreatments();
    }
    
    public ProviderRepresentation() {
        super();
    }
    
    public ProviderRepresentation(ProviderDTO providerDTO, UriInfo uriInfo) {
        this.npi = providerDTO.getNPI();
        this.name = providerDTO.getName();
        this.specialization = SpeicalizationType.fromValue(providerDTO.getSpecialization().value());
        List<Long> treatments = providerDTO.getTreatments();
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
    public ProviderRepresentation(ProviderDTO providerDTO) {
        this.npi = providerDTO.getNPI();
        this.name = providerDTO.getName();
        this.specialization = SpeicalizationType.fromValue(providerDTO.getSpecialization().value());
                
        List<Long> treatments = providerDTO.getTreatments();
        
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
