package edu.stevens.cs548.clinic.service.ejb;

import java.util.Date;

import edu.stevens.cs548.clinic.service.dto.PatientDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;

public interface IPatientService {
	
	public class PatientServiceException extends Exception {
		private static final long serialVersionUID = 1L;

		public PatientServiceException(String m) {
			super(m);
		}
	}
	
	public class PatientNotFoundException extends PatientServiceException {
		private static final long serialVersionUID = 1L;

		public PatientNotFoundException(String m) {
			super(m);
		}
	}
	
	public class TreatmentNotFoundException extends PatientServiceException {
		private static final long serialVersionUID = 1L;

		public TreatmentNotFoundException(String m) {
			super(m);
		}
	}
	
	public long createPatient(String name, Date dob, int age, long patientId) throws PatientServiceException; 

	public PatientDTO getPatientByDbId(long id) throws PatientServiceException;
	
	public PatientDTO getPatientByPatientId(long pid) throws PatientServiceException, PatientNotFoundException;
	
	public PatientDTO[] getPatientByNameDob(String name, Date dob);
	
	public void deletePatient(String name, long id) throws PatientServiceException;
	
	// public void addDrugTreatment(long patientDbId, long providerNPI, String diagnosis, String drug, float dosage) throws PatientNotFoundException;
	
	// public TreatmentDTO[] getTreatments(long id, long[] tids) throws PatientNotFoundException, TreatmentNotFoundException, PatientServiceException;
	
	public TreatmentDTO[] getTreatments(long patientId) throws PatientNotFoundException, PatientServiceException;
	
	public void deleteTreatment(long id, long tid) throws PatientNotFoundException, TreatmentNotFoundException, PatientServiceException;
	
	public String siteInfo();
}
