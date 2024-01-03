package edu.stevens.cs548.clinic.service.web.rest;

import java.util.Date;
import java.util.List;

import edu.stevens.cs548.clinic.domain.ITreatmentVisitor;
import edu.stevens.cs548.clinic.domain.RadDate;
import edu.stevens.cs548.clinic.service.web.rest.data.LinkType;

public class Treatment_PDO_to_Representation_Test implements ITreatmentVisitor {
    
    private TreatmentRepresentation rep;
    
    public Treatment_PDO_to_Representation_Test() {
    }
    public TreatmentRepresentation getRep() {
        return rep;
    }
    
    @Override
    public void visitDrugTreatment(long tid, long patientDbId,
            long providerNPI, String diagnosis, String drug, float dosage) {
        
        rep = new TreatmentRepresentation();
        rep.setId(tid);
        
        String patientURI = Long.toString(patientDbId);
        LinkType patientLink = new LinkType();
        patientLink.setUrl(patientURI);
        patientLink.setRelation(Representation.RELATION_PATIENT);
        patientLink.setMediaType(Representation.MEDIA_TYPE);
        rep.setPatient(patientLink);
        
        String providerURI = Long.toString(providerNPI);
        LinkType providerLink = new LinkType();
        providerLink.setUrl(providerURI);
        providerLink.setRelation(Representation.RELATION_PROVIDER);
        providerLink.setMediaType(Representation.MEDIA_TYPE);
        rep.setProvider(providerLink);
        
        rep.setDiagnosis(diagnosis);
        
        DrugTreatmentRepresentation drugRep = new DrugTreatmentRepresentation();
        drugRep.setDrugName(drug);
        drugRep.setDosage(dosage);
        rep.setDrugTreatment(drugRep);
    }

    @Override
    public void visitRadiologyTreatment(long tid, long patientDbId,
            long providerNPI, String diagnosis, List<RadDate> dates) {
        
        rep = new TreatmentRepresentation();
        rep.setId(tid);
        
        String patientURI = Long.toString(patientDbId);
        LinkType patientLink = new LinkType();
        patientLink.setUrl(patientURI);
        patientLink.setRelation(Representation.RELATION_PATIENT);
        patientLink.setMediaType(Representation.MEDIA_TYPE);
        rep.setPatient(patientLink);
        
        String providerURI = Long.toString(providerNPI);
        LinkType providerLink = new LinkType();
        providerLink.setUrl(providerURI);
        providerLink.setRelation(Representation.RELATION_PROVIDER);
        providerLink.setMediaType(Representation.MEDIA_TYPE);
        rep.setProvider(providerLink);
        
        rep.setDiagnosis(diagnosis);
        
        RadiologyTreatmentRepresentation radiologyRep = new RadiologyTreatmentRepresentation();
        List<Date> dateList = radiologyRep.getDate();
        for(RadDate radDate : dates) {
            dateList.add(radDate.getDate());
        }
        rep.setRadiologyTreatment(radiologyRep);
        
    }

    @Override
    public void visitSurgeryTreatment(long tid, long patientDbId,
            long providerNPI, String diagnosis, Date date) {
        
        rep = new TreatmentRepresentation();
        rep.setId(tid);
        
        String patientURI = Long.toString(patientDbId);
        LinkType patientLink = new LinkType();
        patientLink.setUrl(patientURI);
        patientLink.setRelation(Representation.RELATION_PATIENT);
        patientLink.setMediaType(Representation.MEDIA_TYPE);
        rep.setPatient(patientLink);
        
        String providerURI = Long.toString(providerNPI);
        LinkType providerLink = new LinkType();
        providerLink.setUrl(providerURI);
        providerLink.setRelation(Representation.RELATION_PROVIDER);
        providerLink.setMediaType(Representation.MEDIA_TYPE);
        rep.setProvider(providerLink);
        
        rep.setDiagnosis(diagnosis);
        
        SurgeryTreatmentRepresentation surgeryRep = new SurgeryTreatmentRepresentation();
        surgeryRep.setDate(date);
        rep.setSurgeryTreatment(surgeryRep);
        
    }
}
