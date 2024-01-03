package edu.stevens.cs548.clinic.service.ejb;

import java.util.Date;
import java.util.List;

import edu.stevens.cs548.clinic.domain.ITreatmentVisitor;
import edu.stevens.cs548.clinic.domain.Patient;
import edu.stevens.cs548.clinic.domain.Provider;
import edu.stevens.cs548.clinic.domain.RadDate;
import edu.stevens.cs548.clinic.service.dto.DrugTreatmentType;
import edu.stevens.cs548.clinic.service.dto.ObjectFactory;
import edu.stevens.cs548.clinic.service.dto.RadiologyTreatmentType;
import edu.stevens.cs548.clinic.service.dto.SurgeryTreatmentType;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;

public class TreatmentPDOToDTO implements ITreatmentVisitor {

	private ObjectFactory factory = new ObjectFactory();
	private TreatmentDTO treatmentDTO;
	public TreatmentDTO getTreatmentDTO() {
		return treatmentDTO;
	}
	
	@Override
	public void visitDrugTreatment(long tid, long patientDbId, long providerNPI, String diagnosis, String drug, float dosage) {
		treatmentDTO = factory.createTreatmentDTO();
		treatmentDTO.setId(tid);
		treatmentDTO.setPatientDbId(patientDbId);
		treatmentDTO.setProviderNPI(providerNPI);
		treatmentDTO.setDiagnosis(diagnosis);
		DrugTreatmentType drugTreatment = factory.createDrugTreatmentType();
		drugTreatment.setDrugName(drug);
		drugTreatment.setDosage(dosage);
		treatmentDTO.setDrugTreatment(drugTreatment);
	}

	@Override
	public void visitRadiologyTreatment(long tid, long patientDbId, long providerNPI, String diagnosis, List<RadDate> dates) {
		treatmentDTO = factory.createTreatmentDTO();
		treatmentDTO.setId(tid);
		treatmentDTO.setPatientDbId(patientDbId);
		treatmentDTO.setProviderNPI(providerNPI);
		treatmentDTO.setDiagnosis(diagnosis);
		RadiologyTreatmentType radiologyTreatment = factory.createRadiologyTreatmentType();
		List<Date> dateList = radiologyTreatment.getDate();
		for(RadDate radDate : dates) {
			dateList.add(radDate.getDate());
		}
		treatmentDTO.setRadiologyTreatment(radiologyTreatment);
	}

	@Override
	public void visitSurgeryTreatment(long tid, long patientDbId, long providerNPI, String diagnosis, Date date) {
		treatmentDTO = factory.createTreatmentDTO();
		treatmentDTO.setId(tid);
		treatmentDTO.setPatientDbId(patientDbId);
		treatmentDTO.setProviderNPI(providerNPI);
		treatmentDTO.setDiagnosis(diagnosis);
		SurgeryTreatmentType surgeryTreatmentType = factory.createSurgeryTreatmentType();
		surgeryTreatmentType.setDate(date);
		treatmentDTO.setSurgeryTreatment(surgeryTreatmentType);
	}
	
}
