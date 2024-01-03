package edu.stevens.cs548.clinic.domain;

import java.util.Date;
import java.util.List;

public interface ITreatmentFactory {
	
	public DrugTreatment createDrugTreatment(String diagnosis, String drug, float dosage, Patient patient);
	
	public RadiologyTreatment createRadiologyTreatment(String diagnosis, List<Date> dates, Patient patient);
	
	public SurgeryTreatment createSurgeryTreatment(String diagnosis, Date date, Patient patient);
	
}
