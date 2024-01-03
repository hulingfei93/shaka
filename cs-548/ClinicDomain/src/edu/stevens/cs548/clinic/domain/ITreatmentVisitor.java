package edu.stevens.cs548.clinic.domain;

import java.util.Date;
import java.util.List;

public interface ITreatmentVisitor {
    
    public void visitDrugTreatment(long tid, long patientDbId, long providerNPI, String diagnosis, String drug, float dosage);
    
    public void visitRadiologyTreatment(long tid, long patientDbId, long providerNPI, String diagnosis, List<RadDate> dates);
    
    public void visitSurgeryTreatment(long tid, long patientDbId, long providerNPI, String diagnosis, Date date);
    
}
