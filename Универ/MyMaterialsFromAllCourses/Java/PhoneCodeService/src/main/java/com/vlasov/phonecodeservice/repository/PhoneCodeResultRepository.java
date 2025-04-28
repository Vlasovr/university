package com.vlasov.phonecodeservice.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import com.vlasov.phonecodeservice.model.PhoneCodeResult;
import java.util.List;

@Repository
public interface PhoneCodeResultRepository extends JpaRepository<PhoneCodeResult, Long> {}