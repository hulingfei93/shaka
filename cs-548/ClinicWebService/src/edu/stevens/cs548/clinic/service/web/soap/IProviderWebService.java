package edu.stevens.cs548.clinic.service.web.soap;

import javax.jws.WebMethod;
import javax.jws.WebService;

import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.PatientNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderServiceException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.TreatmentNotFoundException;

@WebService(
		name="IProviderWebPort",
		targetNamespace="http://cs548.stevens.edu/clinic/service/web/soap/provider")
public interface IProviderWebService {
	
	@WebMethod(operationName="create")
	public long createProvider(String name, long NPI) throws ProviderServiceException; 
	
	@WebMethod
	public ProviderDTO[] getProviderByName(String name);
	
	@WebMethod
	public ProviderDTO getProviderByNPI(long NPI) throws ProviderNotFoundException, ProviderServiceException;
	
	@WebMethod
	public long addTreatment(long patientDbId, long NPI, TreatmentDTO treatment) throws PatientNotFoundException, ProviderNotFoundException, ProviderServiceException;
	
	@WebMethod
	public void deleteTreatment(long tid, long patientDbId, long NPI) throws TreatmentNotFoundException, ProviderServiceException;
	
	@WebMethod
	public TreatmentDTO[] getTreatmentsBy_PatientDbId_NPI(long patientDbId, long NPI) throws ProviderNotFoundException, ProviderServiceException;
	
	@WebMethod
	public TreatmentDTO[] getTreatments(long NPI) throws ProviderNotFoundException, ProviderServiceException;
}
